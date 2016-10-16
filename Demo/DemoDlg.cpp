
// DemoDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDemoDlg �Ի���




CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//����ר��
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


// CDemoDlg ��Ϣ�������

BOOL CDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ctrlSlider.ShowWindow(FALSE); //�ڲػ���
	m_ctrlSlider.SetRange(0,255); //���û���λ�õ����ֵ����Сֵ
	m_ctrlSlider.SetPos(255);          //���û����Ĭ�ϵ�ǰλ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


IplImage *pImg = NULL;
IplImage *pGray = NULL;
IplImage *pContrastImage = NULL;
void CDemoDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(true, NULL, L"", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		L"image files(*.bmp; *.jpg)|*.bmp; *.jpg|All Files(*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		char * pstr= new char[256]; //����ռ�;
		size_t sz=0;
		wcstombs_s(&sz,pstr,256,dlg.GetPathName(),_TRUNCATE);

		pImg = cvLoadImage(pstr);
		pGray = cvLoadImage(pstr,CV_LOAD_IMAGE_GRAYSCALE); 
		// 		pGray = cvCreateImage(cvGetSize(pImg), pImg->depth, 1);//����Ŀ��ͼ��
		// 		cvCvtColor(pImg,pGray,CV_BGR2GRAY);  //ͼ��ҶȻ�
		if(!pImg)
		{
			return;
		}
		if( !pImg->imageData ) // ����Ƿ���ȷ����ͼ��
		{
			return;
		}
		cvDestroyAllWindows();
		cvNamedWindow("ԭͼ", CV_WINDOW_AUTOSIZE);
		cvShowImage("ԭͼ",pImg);  //��ʾԭͼƬ
	}
}

//ͼ��ҶȻ�
void CDemoDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pImg == NULL)
	{
		MessageBox(L"���һ��ͼƬ");
		return;
	}

	IplImage* pDstGray=cvCreateImage(cvGetSize(pImg), pImg->depth, 1); //����Ŀ��ͼ��
	uchar* data=(uchar*)pImg->imageData; //ȡԭͼ�������
	int step = pImg->widthStep/sizeof(uchar);     
	int channels = pImg->nChannels;               // ԭΪ3ͨ��
	uchar b=0,g=0,r=0;
	for(int i=0; i<pImg->height; i++)
	{
		for(int j=0; j<pImg->width; j++)
		{
			b=data[i*step+j*channels+0];   // ��ʱ����ͨ������bgr��ֵ�ﵽ����Ч����
			g=data[i*step+j*channels+1];
			r=data[i*step+j*channels+2];
			((char*)(pDstGray->imageData + i*pDstGray->widthStep))[j] = 0.11*b+0.3*r+0.59*g;
			//i*step     ��i=0 ��Ϊ��ͼ�ĵ�һ��   Ϊ1���ǵڶ��� 
			//j*chanels+0  j*ͨ������j=0Ϊ��һ�еĵ�0��ͨ��->b  
			//j*channels+1         ��j=1Ϊ�ڶ��еĵ�1��ͨ��->g
		}
	}
	cvDestroyAllWindows();  //���ٴ���
	cvNamedWindow("�Ҷ�ͼ");
	cvShowImage("�Ҷ�ͼ",pDstGray);

	cvWaitKey();
	cvDestroyWindow("�Ҷ�ͼ");  //���ٴ���
	cvReleaseImage(&pDstGray);  //�ͷ�ͼ��
}


//�Աȶ�
void CDemoDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pImg == NULL)
	{
		MessageBox(L"���һ��ͼƬ");
		return;
	}

	CString str;
	if(GetDlgItemText(IDC_BUTTON3,str),str==L"�Աȶ�")
	{
		SetDlgItemText(IDC_BUTTON3,L"��ʾԭͼ");
		m_ctrlSlider.ShowWindow(TRUE);//��ʾ�ÿؼ�
		cvDestroyAllWindows();
	}
	else
	{
		SetDlgItemText(IDC_BUTTON3,L"�Աȶ�");
		m_ctrlSlider.ShowWindow(FALSE);
		cvDestroyAllWindows();
		cvNamedWindow("ԭͼ");
		cvShowImage("ԭͼ",pImg);
	}
}

//�Աȶ�
void CDemoDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nPos = m_ctrlSlider.GetPos(); //��û���ĵ�ǰλ��

	if (NULL == pGray)
	{
		MessageBox(L"���һ��ͼƬ");
		return;
	}

	pContrastImage = cvCreateImage(cvGetSize(pGray), pGray->depth, 1);
	//p[]���ͼ������Ҷȼ��ĳ��ָ��ʣ�
	//p1[]��Ÿ����Ҷȼ�֮ǰ�ĸ��ʺͣ�����ֱ��ͼ�任��
	//num[]���ͼ������Ҷȼ����ֵĴ���;
	float p[256],p1[256],num[256];

	//�����������
	memset(p,0,sizeof(p));
	memset(p1,0,sizeof(p1));
	memset(num,0,sizeof(num));

	int height=pGray->height;
	int width=pGray->width;
	long size = height * width;

	//����ͼ������Ҷȼ����ֵĴ���
	for(int x=0; x<height; x++)
	{
		for(int y=0; y<width; y++)
		{
			uchar v=((uchar*)(pGray->imageData + pGray->widthStep*y))[x]; 
			num[v]++;
		}
	}

	//����ͼ������Ҷȼ��ĳ��ָ���
	for(int i=0;i<256;i++)
	{
		p[i]=num[i]/size;
	}

	//���Ÿ����Ҷȼ�֮ǰ�ĸ��ʺ�
	for(int i=0;i<256;i++)
	{
		for(int k=0;k<=i;k++)
			p1[i]+=p[k];
	}

	//�ѻҶ�ֵ���쵽0-�Ա���ֵ
	for(int x=0; x<height; x++)
	{
		for(int y=0; y<width; y++)
		{
			uchar v=((uchar*)(pGray->imageData + pGray->widthStep*x))[y];
			((uchar*)(pContrastImage->imageData + pContrastImage->widthStep*x))[y]= p1[v]*nPos+0.5;            
		}
	}

	cvNamedWindow("�Աȶ�ͼ");
	cvShowImage("�Աȶ�ͼ", pContrastImage);

	cvNamedWindow("�Աȶ�ͼ");
	cvShowImage("�Աȶ�ͼ", pContrastImage);

	*pResult = 0;
}


void CDemoDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cvDestroyAllWindows();
	CDialogEx::OnCancel();
}


//�����䷽����ֵ
void CDemoDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IplImage* pGray = cvCreateImage(cvGetSize(pImg), pImg->depth, 1);//����Ŀ��ͼ��
	cvCvtColor(pImg,pGray,CV_BGR2GRAY);  //ͼ��ҶȻ�

	IplImage* pdst=cvCreateImage(cvGetSize(pGray),IPL_DEPTH_8U,1);  

	int width = pGray->width;
	int height = pGray->height;
	int pixelCount[256] = {0};
	float pixelPro[256] = {0};
	int i, j, pixelSum = width * height, threshold = 0;
	uchar* data = (uchar*)pGray->imageData;
	int step = pGray->widthStep/sizeof(uchar);

	//ͳ��ÿ���Ҷȼ������صĸ���   
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[(int)data[i * width + j]]++;
		}
	}

	//����ÿ���Ҷȼ���������Ŀռ����ͼ��ı���   
	for (i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
	}

	//�����Ҷȼ�[0,255],Ѱ�Һ��ʵ�threshold   
	//ǰ�������ص���ռ����ͼ��ı�����Ϊ��0����ƽ���ҶȦ�0
	//�������ص���ռ����ͼ��ı���Ϊ��1,��ƽ���Ҷ�Ϊ��1
	float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;
		for (j = 0; j < 256; j++)
		{
			if (j <= i)   //��������   
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}

			else   //ǰ������   
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

	// ���������ֵ��ԭͼ�����ָ��
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
	cvNamedWindow("�����䷽����ֵ�ָ�");
	cvShowImage( "�����䷽����ֵ�ָ�", pGray );

	cvWaitKey(0); 
	cvDestroyWindow( "�����䷽����ֵ�ָ�" );
	cvReleaseImage( &pGray );
}

//������ȡ
void CDemoDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pGray == NULL)
	{
		MessageBox(L"���һ��ͼƬ");
		return;
	}
	
	Mat lbp(pGray->width, pGray->height, CV_8UC1);    //����һ����pGray�ȸߵȿ�ĵ�ͨ��ͼ��lbp  
	Mat Image(pGray,0);
	
	unsigned center = 0;                                     //��ȡ��Ҫ����LBPֵ�����ĵ�ĻҶ�ֵ  
	unsigned center_lbp = 0;                                 //����center����LBPֵ  
	//����LBPͼ��  
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
			lbp.at<uchar>(row, col) = center_lbp;          //��center������õ�LBPֵ�����lbpͼ�����Ӧλ��  
		}  
	}  
	
	
	IplImage qImg;
	qImg = IplImage(lbp); //һ�әn���]�}�u������qImg.imageData����ָ��pImg->imageData 
	cvNamedWindow("������ȡ",CV_WINDOW_AUTOSIZE);
	cvShowImage("������ȡ",&IplImage(lbp)); 
	
}


//ģ��ƥ��ͼ��ʶ��
void CDemoDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IplImage* pGray = cvLoadImage("D:\\C++Project\\Demo\\test\\face.PNG",CV_LOAD_IMAGE_GRAYSCALE); 
	IplImage* pImg2 = cvLoadImage("D:\\C++Project\\Demo\\test\\sample.PNG",CV_LOAD_IMAGE_GRAYSCALE);

	uchar* data1=(uchar*)pGray->imageData;
	uchar* data2=(uchar*)pImg2->imageData;
	int step1 = pGray->widthStep/sizeof(uchar);  
	int step2 = pImg2->widthStep/sizeof(uchar);

	//ȡƽ������Сֵ
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
					//ƽ����ƥ�䷨
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

	cvNamedWindow("ͼ��ʶ��");
	cvShowImage( "ͼ��ʶ��", pGray);

	cvWaitKey(0); 
	cvDestroyWindow("ͼ��ʶ��");
	cvReleaseImage( &pGray );
}

//ͼ��ƴ��
void CDemoDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

				//ƽ����ƥ�䷨
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
	IplImage* pdst = cvCreateImage(osize, pImg1->depth, 1);//����Ŀ��ͼ��

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

	cvNamedWindow("ԭͼ1");
	cvShowImage( "ԭͼ1", pImg1);
	cvNamedWindow("ԭͼ2");
	cvShowImage( "ԭͼ2", pImg2);
	cvNamedWindow("ͼ��ƴ��");
	cvShowImage( "ͼ��ƴ��", pdst);

	cvWaitKey(0); 
	cvDestroyWindow("ԭͼ1");
	cvReleaseImage( &pImg1 );
	cvDestroyWindow("ԭͼ2");
	cvReleaseImage( &pImg2 );
	cvDestroyWindow("ͼ��ƴ��");
	cvReleaseImage( &pdst );
}
