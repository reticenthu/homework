
// DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include "windows.h"
#include "fstream"
#include <iostream>
using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void getHOG(IplImage* pSrcImage,bool bshow,vector<float> &featureVec);
void func(int i_x, int i_y, int i_w, IplImage* Img_in, float* fbin);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDemoDlg 对话框




CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//滑块专用
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlSlider);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDemoDlg::OnBnClickedButton2)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDemoDlg::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDemoDlg::OnBnClickedButton3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDemoDlg::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDCANCEL, &CDemoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON5, &CDemoDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDemoDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDemoDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON4, &CDemoDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDemoDlg 消息处理程序

BOOL CDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ctrlSlider.ShowWindow(FALSE); //掩藏滑块
	m_ctrlSlider.SetRange(0,255); //设置滑块位置的最大值和最小值
	m_ctrlSlider.SetPos(255);          //设置滑块的默认当前位置

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


IplImage *pImg = NULL;
IplImage *pGray = NULL;
IplImage *pContrastImage = NULL;
void CDemoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(true, NULL, L"", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		L"image files(*.bmp; *.jpg)|*.bmp; *.jpg|All Files(*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		char * pstr= new char[256]; //分配空间;
		size_t sz=0;
		wcstombs_s(&sz,pstr,256,dlg.GetPathName(),_TRUNCATE);

		pImg = cvLoadImage(pstr);
		pGray = cvLoadImage(pstr,CV_LOAD_IMAGE_GRAYSCALE); 
		// 		pGray = cvCreateImage(cvGetSize(pImg), pImg->depth, 1);//创建目标图像
		// 		cvCvtColor(pImg,pGray,CV_BGR2GRAY);  //图像灰度化
		if(!pImg)
		{
			return;
		}
		if( !pImg->imageData ) // 检查是否正确载入图像
		{
			return;
		}
		cvDestroyAllWindows();
		cvNamedWindow("原图", CV_WINDOW_AUTOSIZE);
		cvShowImage("原图",pImg);  //显示原图片
	}
}

//图像灰度化
void CDemoDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (pImg == NULL)
	{
		MessageBox(L"请打开一张图片");
		return;
	}

	IplImage* pDstGray=cvCreateImage(cvGetSize(pImg), pImg->depth, 1); //创建目标图像
	uchar* data=(uchar*)pImg->imageData; //取原图像的数据
	int step = pImg->widthStep/sizeof(uchar);     
	int channels = pImg->nChannels;               // 原为3通道
	uchar b=0,g=0,r=0;
	for(int i=0; i<pImg->height; i++)
	{
		for(int j=0; j<pImg->width; j++)
		{
			b=data[i*step+j*channels+0];   // 此时可以通过更改bgr的值达到访问效果。
			g=data[i*step+j*channels+1];
			r=data[i*step+j*channels+2];
			((char*)(pDstGray->imageData + i*pDstGray->widthStep))[j] = 0.11*b+0.3*r+0.59*g;
			//i*step     当i=0 即为上图的第一行   为1就是第二行 
			//j*chanels+0  j*通道数当j=0为第一列的第0个通道->b  
			//j*channels+1         当j=1为第二列的第1个通道->g
		}
	}
	cvDestroyAllWindows();  //销毁窗口
	cvNamedWindow("灰度图");
	cvShowImage("灰度图",pDstGray);

	cvWaitKey();
	cvDestroyWindow("灰度图");  //销毁窗口
	cvReleaseImage(&pDstGray);  //释放图像
}


//对比度
void CDemoDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (pImg == NULL)
	{
		MessageBox(L"请打开一张图片");
		return;
	}

	CString str;
	if(GetDlgItemText(IDC_BUTTON3,str),str==L"对比度")
	{
		SetDlgItemText(IDC_BUTTON3,L"显示原图");
		m_ctrlSlider.ShowWindow(TRUE);//显示该控件
		cvDestroyAllWindows();
	}
	else
	{
		SetDlgItemText(IDC_BUTTON3,L"对比度");
		m_ctrlSlider.ShowWindow(FALSE);
		cvDestroyAllWindows();
		cvNamedWindow("原图");
		cvShowImage("原图",pImg);
	}
}

//对比度
void CDemoDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置

	if (NULL == pGray)
	{
		MessageBox(L"请打开一张图片");
		return;
	}

	pContrastImage = cvCreateImage(cvGetSize(pGray), pGray->depth, 1);
	//p[]存放图像各个灰度级的出现概率；
	//p1[]存放各个灰度级之前的概率和，用于直方图变换；
	//num[]存放图象各个灰度级出现的次数;
	float p[256],p1[256],num[256];

	//清空三个数组
	memset(p,0,sizeof(p));
	memset(p1,0,sizeof(p1));
	memset(num,0,sizeof(num));

	int height=pGray->height;
	int width=pGray->width;
	long size = height * width;

	//求存放图象各个灰度级出现的次数
	for(int x=0; x<height; x++)
	{
		for(int y=0; y<width; y++)
		{
			uchar v=((uchar*)(pGray->imageData + pGray->widthStep*y))[x]; 
			num[v]++;
		}
	}

	//求存放图像各个灰度级的出现概率
	for(int i=0;i<256;i++)
	{
		p[i]=num[i]/size;
	}

	//求存放各个灰度级之前的概率和
	for(int i=0;i<256;i++)
	{
		for(int k=0;k<=i;k++)
			p1[i]+=p[k];
	}

	//把灰度值拉伸到0-对比阈值
	for(int x=0; x<height; x++)
	{
		for(int y=0; y<width; y++)
		{
			uchar v=((uchar*)(pGray->imageData + pGray->widthStep*x))[y];
			((uchar*)(pContrastImage->imageData + pContrastImage->widthStep*x))[y]= p1[v]*nPos+0.5;            
		}
	}

	cvNamedWindow("对比度图");
	cvShowImage("对比度图", pContrastImage);

	cvNamedWindow("对比度图");
	cvShowImage("对比度图", pContrastImage);

	*pResult = 0;
}


void CDemoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	cvDestroyAllWindows();
	CDialogEx::OnCancel();
}


//最大类间方差阈值
void CDemoDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	IplImage* pGray = cvCreateImage(cvGetSize(pImg), pImg->depth, 1);//创建目标图像
	cvCvtColor(pImg,pGray,CV_BGR2GRAY);  //图像灰度化

	IplImage* pdst=cvCreateImage(cvGetSize(pGray),IPL_DEPTH_8U,1);  

	int width = pGray->width;
	int height = pGray->height;
	int pixelCount[256] = {0};
	float pixelPro[256] = {0};
	int i, j, pixelSum = width * height, threshold = 0;
	uchar* data = (uchar*)pGray->imageData;
	int step = pGray->widthStep/sizeof(uchar);

	//统计每个灰度级中像素的个数   
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[(int)data[i * width + j]]++;
		}
	}

	//计算每个灰度级的像素数目占整幅图像的比例   
	for (i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
	}

	//遍历灰度级[0,255],寻找合适的threshold   
	//前景的像素点数占整幅图像的比例记为ω0，其平均灰度μ0
	//背景像素点数占整幅图像的比例为ω1,其平均灰度为μ1
	float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;
		for (j = 0; j < 256; j++)
		{
			if (j <= i)   //背景部分   
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}

			else   //前景部分   
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		deltaTmp = (float)(w0 *w1* pow((u0 - u1), 2));
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}

	// 利用最佳阈值对原图像作分割处理
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if (data[i*step+j] > threshold)
			{
				data[i*step+j] = 255;
			}
			else
			{
				data[i*step+j] = 0;
			}
		}
	}

	cvDestroyAllWindows();
	cvNamedWindow("最大类间方差阈值分割");
	cvShowImage( "最大类间方差阈值分割", pGray );

	cvWaitKey(0); 
	cvDestroyWindow( "最大类间方差阈值分割" );
	cvReleaseImage( &pGray );
}

//特征提取
void CDemoDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	if(pGray == NULL)
	{
		MessageBox(L"请打开一张图片");
		return;
	}
	
	Mat lbp(pGray->width, pGray->height, CV_8UC1);    //建立一个与pGray等高等宽的单通道图像lbp  
	Mat Image(pGray,0);
	
	unsigned center = 0;                                     //提取需要计算LBP值得中心点的灰度值  
	unsigned center_lbp = 0;                                 //计算center处的LBP值  
	//计算LBP图像  
	for (int row = 1; row <pGray->width-1; row++)              
	{  
		for (int col = 1; col < pGray->height-1; col++)  
		{  
			center = Image.at<uchar>(row, col); 
			center_lbp = 0;  

			if (center <= Image.at<uchar>(row - 1, col - 1))  
				center_lbp += 128;  

			if (center <= Image.at<uchar>(row - 1, col))  
				center_lbp += 64;  

			if (center <= Image.at<uchar>(row - 1, col + 1))  
				center_lbp += 32;  

			if (center <= Image.at<uchar>(row, col - 1))  
				center_lbp += 16;  

			if (center <= Image.at<uchar>(row, col + 1))  
				center_lbp += 8;  

			if (center <= Image.at<uchar>(row + 1, col - 1))  
				center_lbp += 4;  

			if (center <= Image.at<uchar>(row + 1, col))  
				center_lbp += 2;  

			if (center <= Image.at<uchar>(row + 1, col + 1))  
				center_lbp += 1;  
			lbp.at<uchar>(row, col) = center_lbp;          //把center处计算好的LBP值存放在lbp图像的相应位置  
		}  
	}  
	
	
	IplImage qImg;
	qImg = IplImage(lbp); //一樣檔案沒複製，所以qImg.imageData就是指向pImg->imageData 
	cvNamedWindow("特征提取",CV_WINDOW_AUTOSIZE);
	cvShowImage("特征提取",&IplImage(lbp)); 
	
}


//模板匹配图像识别
void CDemoDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	IplImage* pGray = cvLoadImage("D:\\C++Project\\Demo\\test\\face.PNG",CV_LOAD_IMAGE_GRAYSCALE); 
	IplImage* pImg2 = cvLoadImage("D:\\C++Project\\Demo\\test\\sample.PNG",CV_LOAD_IMAGE_GRAYSCALE);

	uchar* data1=(uchar*)pGray->imageData;
	uchar* data2=(uchar*)pImg2->imageData;
	int step1 = pGray->widthStep/sizeof(uchar);  
	int step2 = pImg2->widthStep/sizeof(uchar);

	//取平方差最小值
	long long lmin;
	int x = 0;
	int y = 0;
	for(int i=0; i<(pGray->height - pImg2->height); i++)
	{
		for(int j=0; j<(pGray->width - pImg2->width); j++)
		{
			long long ivalue = 0;
			for(int n=0; n<pImg2->height; n++)
			{
				for(int m=0; m<pImg2->width; m++)
				{
					//平方差匹配法
					long aa = data1[(i+n)*step1+j+m];
					long bb = data2[n*step2+m];

					ivalue += abs(aa*aa-bb*bb);
				}
			}

			if ((0==i && 0==j) || (ivalue < lmin))
			{
				lmin = ivalue;
				x = j;
				y = i;
			}
		}
	}

	CvRect tempRect = cvRect(x, y, pImg2->width, pImg2->height);

	cvRectangle(pGray, cvPoint(tempRect.x,tempRect.y),
		cvPoint(tempRect.x+tempRect.width,tempRect.y+tempRect.height),CV_RGB(255,0,0), 2);

	cvNamedWindow("图像识别");
	cvShowImage( "图像识别", pGray);

	cvWaitKey(0); 
	cvDestroyWindow("图像识别");
	cvReleaseImage( &pGray );
}

//图像拼接
void CDemoDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	IplImage* pImg1 = cvLoadImage("D:\\C++Project\\Demo\\test\\left.PNG",CV_LOAD_IMAGE_GRAYSCALE); 
	IplImage* pImg2 = cvLoadImage("D:\\C++Project\\Demo\\test\\right.PNG",CV_LOAD_IMAGE_GRAYSCALE);

	uchar* data1=(uchar*)pImg1->imageData;
	uchar* data2=(uchar*)pImg2->imageData;
	int step1 = pImg1->widthStep/sizeof(uchar);  
	int step2 = pImg2->widthStep/sizeof(uchar);

	long long lmin;
	int x = 0;
	int y = 0;
	for(int i=0; i<(pImg1->height - pImg2->height); i++)
	{
		for(int j=0; j<pImg1->width; j++)
		{
			long long ivalue = 0;
			for(int n=0; n<pImg2->height; n++)
			{

				//平方差匹配法
				long aa = data1[(i+n)*step1+j];
				long bb = data2[n*step2];

				ivalue += abs(aa*aa-bb*bb);

			}
			if ((0==i && 0==j) || (ivalue < lmin))
			{
				lmin = ivalue;
				x = j;
				y = i;
			}
		}
	}

	CvSize osize;
	osize.width = x+pImg2->width;
	osize.height = pImg2->height;
	IplImage* pdst = cvCreateImage(osize, pImg1->depth, 1);//创建目标图像

	for(int n=0; n<pdst->height; n++)
	{
		for(int m=0; m<pdst->width; m++)
		{
			if (m < x)
			{
				((char*)(pdst->imageData + n*pdst->widthStep))[m] = data1[(y+n)*pImg1->widthStep+m];
			}
			else
			{
				((char*)(pdst->imageData + n*pdst->widthStep))[m] = data2[n*pImg2->widthStep+m-x];
			}

		}
	}

	cvNamedWindow("原图1");
	cvShowImage( "原图1", pImg1);
	cvNamedWindow("原图2");
	cvShowImage( "原图2", pImg2);
	cvNamedWindow("图像拼接");
	cvShowImage( "图像拼接", pdst);

	cvWaitKey(0); 
	cvDestroyWindow("原图1");
	cvReleaseImage( &pImg1 );
	cvDestroyWindow("原图2");
	cvReleaseImage( &pImg2 );
	cvDestroyWindow("图像拼接");
	cvReleaseImage( &pdst );
}
