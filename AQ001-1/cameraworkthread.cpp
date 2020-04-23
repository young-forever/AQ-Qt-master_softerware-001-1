#include "cameraworkthread.h"
#include "MvCameraControl.h"
#include <iostream>
#include <sstream>
#include "mainwindow.h"
#include <memory>

#include <sys/time.h>
struct timeval tpstart,tpend;
double timeuse;

using namespace std;

bool CameraWorkFlag = false;



CameraWorkThread::CameraWorkThread(QObject* par) : QThread(par)
{
    m_isCanRun = true;
    image_cnt = 0;

}

CameraWorkThread::~CameraWorkThread()
{

}

void CameraWorkThread::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
    cout<<"warning: camera thread close!!"<<endl;
}

void PressEnterToExit(void)
{
    int c;
    while ( (c = getchar()) != '\n' && c != EOF );
    fprintf( stderr, "\nPress enter to exit.\n");
    while( getchar() != '\n');
}

bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
    if (NULL == pstMVDevInfo)
    {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
    {
        int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
        printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
        printf("CurrentIp: %d.%d.%d.%d\n" , nIp1, nIp2, nIp3, nIp4);
        printf("UserDefinedName: %s\n\n" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
    {
        printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chModelName);
        printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
    }
    else
    {
        printf("Not support.\n");
    }

    return true;
}


unsigned char *pDataForSaveImage = NULL;
//该线程执行  任务
void CameraWorkThread::run()
{
    int nRet = MV_OK;

    void* handle = NULL;
    //    unsigned char * pData = NULL;
    //    unsigned char *pDataForRGB = NULL;


    MV_CC_DEVICE_INFO_LIST stDeviceList;
    memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    do
    {
        // 枚举设备
        // enum device
        nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
        if (MV_OK != nRet)
        {
            printf("MV_CC_EnumDevices fail! nRet [%x]\n", nRet);
            stopImmediately();
            break;
        }
        if (stDeviceList.nDeviceNum > 0)
        {
            for (int i = 0; i < stDeviceList.nDeviceNum; i++)
            {
                printf("[device %d]:\n", i);
                MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
                if (NULL == pDeviceInfo)
                {
                    break;
                }
                PrintDeviceInfo(pDeviceInfo);
            }
        }
        else
        {
            printf("Find No Devices!\n");
            stopImmediately();
            break;
        }

        //    printf("Please Intput camera index: ");
        unsigned int nIndex = CAMERA_INDEX;
        //    scanf("%d", &nIndex);

        if (nIndex >= stDeviceList.nDeviceNum)
        {
            printf("Intput error!\n");
            stopImmediately();
            break;
        }

        // 选择设备并创建句柄
        // select device and create handle
        nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
        if (MV_OK != nRet)
        {
            printf("MV_CC_CreateHandle fail! nRet [%x]\n", nRet);
            stopImmediately();
            break;
        }

        // 打开设备
        // open device
        nRet = MV_CC_OpenDevice(handle);
        if (MV_OK != nRet)
        {
            printf("MV_CC_OpenDevice fail! nRet [%x]\n", nRet);
            stopImmediately();
            break;
        }

        // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
        if (stDeviceList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
        {
            int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
            if (nPacketSize > 0)
            {
                nRet = MV_CC_SetIntValue(handle,"GevSCPSPacketSize",nPacketSize);
                if(nRet != MV_OK)
                {
                    printf("Warning: Set Packet Size fail nRet [0x%x]!\n", nRet);
                }
            }
            else
            {
                printf("Warning: Get Packet Size fail nRet [0x%x]!\n", nPacketSize);
            }
        }

        nRet = MV_CC_SetBoolValue(handle, "AcquisitionFrameRateEnable", false);
        if (MV_OK != nRet)
        {
            printf("set AcquisitionFrameRateEnable fail! nRet [%x]\n", nRet);
            stopImmediately();
            break;
        }

        // 设置触发模式为on
        // set trigger mode as on
        nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 1);
        if (MV_OK != nRet)
        {
            printf("MV_CC_SetTriggerMode fail! nRet [%x]\n", nRet);
            stopImmediately();
            break;
        }

        // 设置触发源
        // set trigger source
        //    nRet = MV_CC_SetEnumValue(handle, "TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);

        nRet = MV_CC_SetEnumValue(handle, "TriggerSource", MV_TRIGGER_SOURCE_LINE0);
        if (MV_OK != nRet)
        {
            printf("MV_CC_SetTriggerSource fail! nRet [%x]\n", nRet);
            stopImmediately();
            break;
        }

        // 开始取流
        // start grab image
        nRet = MV_CC_StartGrabbing(handle);
        if (MV_OK != nRet)
        {
            printf("MV_CC_StartGrabbing fail! nRet [%x]\n", nRet);
            stopImmediately();
            break;
        }

        //    nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
        //    if (MV_OK != nRet)
        //    {
        //        printf("MV_CC_SetTriggerMode fail! nRet [%x]\n", nRet);
        //        stopImmediately();
        //    }

        //        // ch:获取数据包大小 | en:Get payload size
        // ch:获取数据包大小 | en:Get payload size
        MVCC_INTVALUE stParam;
        memset(&stParam, 0, sizeof(MVCC_INTVALUE));
        nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
        if (MV_OK != nRet)
        {
            printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
            return;
        }

        MV_FRAME_OUT_INFO_EX stImageInfo = {0};
        memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
        unsigned char * pData = (unsigned char *)malloc(sizeof(unsigned char) * stParam.nCurValue);
        if (NULL == pData)
        {
            return;
        }

        //    nRet = MV_CC_SetCommandValue(handle, "TriggerSoftware");
        //    if(MV_OK != nRet)
        //    {
        //        printf("failed in TriggerSoftware[%x]\n", nRet);
        //    }
        do {

            if(CameraWorkFlag == true)
            {
                                gettimeofday(&tpstart,NULL);

                image_cnt++;

                //            cout<<"get image!"<<endl;

                //SOFT TRIGGER
                //            nRet = MV_CC_SetCommandValue(handle, "TriggerSoftware");
                //            if(MV_OK != nRet)
                //            {
                //                printf("failed in TriggerSoftware[%x]\n", nRet);
                //            }

                p_theta = theta;
                dp_theta = d_theta;

                unsigned int nDataSize = stParam.nCurValue;


                nRet = MV_CC_GetOneFrameTimeout(handle, pData, nDataSize, &stImageInfo, 1000);

                gettimeofday(&tpend,NULL);
                timeuse = (1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec)/1000000.0;
                cout << "time use 1:"<<timeuse <<"s."<<endl;
                nRet = MV_CC_GetOneFrameTimeout(handle, pData, nDataSize, &stImageInfo, 1000);
                gettimeofday(&tpend,NULL);
                timeuse = (1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec)/1000000.0;
                cout << "time use 2:"<<timeuse <<"s."<<endl;


                if (nRet == MV_OK)
                {
                    printf("Now you GetOneFrame, Width[%d], Height[%d], nFrameNum[%d]\n\n",
                           stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);

                    // 处理图像
                    // image processing
                    printf("input 0 to do nothing, 1 to convert RGB, 2 to save as BMP\n");
                    //            scanf("%d", &nInput);

                   static  unsigned char*  m_pDataForSaveImage = (unsigned char*)malloc(stImageInfo.nWidth * stImageInfo.nHeight * 4 + 2048);
//                    shared_ptr<unsigned char>  m_pDataForSaveImage = make_shared<unsigned char>(stImageInfo.nWidth * stImageInfo.nHeight * 4 + 2048);


                    if (NULL == m_pDataForSaveImage)
                    {
                        break;
                    }
                    // 填充存图参数
                    // fill in the parameters of save image
                    MV_SAVE_IMAGE_PARAM_EX stSaveParam;
                    memset(&stSaveParam, 0, sizeof(MV_SAVE_IMAGE_PARAM_EX));
                    // 从上到下依次是：输出图片格式，输入数据的像素格式，提供的输出缓冲区大小，图像宽，
                    // 图像高，输入数据缓存，输出图片缓存，JPG编码质量
                    // Top to bottom are：
                    stSaveParam.enImageType = MV_Image_Bmp;
                    stSaveParam.enPixelType = stImageInfo.enPixelType;
                    stSaveParam.nBufferSize = stImageInfo.nWidth * stImageInfo.nHeight * 4 + 2048;
                    stSaveParam.nWidth      = stImageInfo.nWidth;
                    stSaveParam.nHeight     = stImageInfo.nHeight;
                    stSaveParam.pData       = pData;
                    stSaveParam.nDataLen    = stImageInfo.nFrameLen;
                    stSaveParam.pImageBuffer = m_pDataForSaveImage;
                    stSaveParam.nJpgQuality = 80;

                    nRet = MV_CC_SaveImageEx2(handle, &stSaveParam);
                    if(MV_OK != nRet)
                    {
                        printf("failed in MV_CC_SaveImage,nRet[%x]\n", nRet);
                        break;
                    }

//                    static unsigned char* p_buffer = new unsigned char[stImageInfo.nWidth * stImageInfo.nHeight * 4 + 2048];
//                    memcpy(p_buffer,m_pDataForSaveImage,stImageInfo.nWidth * stImageInfo.nHeight * 4 + 2048);
                    pDataForSaveImage = m_pDataForSaveImage;

                    //                string str1 = "image/image";
                    //                stringstream ss;
                    //                ss<<image_cnt;
                    //                str1.append(ss.str());
                    //                str1.append(".bmp");

                    //                FILE* fp = fopen(str1.c_str(), "wb");
                    //                if (NULL == fp)
                    //                {
                    //                    printf("fopen failed\n");
                    //                    break;
                    //                }
                    //                fwrite(pDataForSaveImage, 1, stSaveParam.nImageLen, fp);
                    //                fclose(fp);
                    //                printf("save image succeed\n");

                    emit show_image();
                }
                else
                {
                    cout<<"no image!"<<endl;
                }


                CameraWorkFlag = false;

                gettimeofday(&tpend,NULL);
                timeuse = (1000000*(tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec)/1000000.0;
                cout << "time use:"<<timeuse <<"s."<<endl;
            }

            QMutexLocker locker(&m_lock);

            if(!m_isCanRun)
            {
                // 停止取流
                // end grab image
                nRet = MV_CC_StopGrabbing(handle);
                if (MV_OK != nRet)
                {
                    printf("MV_CC_StopGrabbing fail! nRet [%x]\n", nRet);
                    break;
                }

                return;
            }

        } while (1);
    }while(0);

}


int CameraWorkThread::get_image_cnt(void)
{
    return image_cnt;

}
