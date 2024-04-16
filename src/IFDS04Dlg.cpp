
// IFDS04Dlg.cpp : implementation file
//
#include "pch.h"
#include "framework.h"
#include "IFDS04.h"
#include "IFDS04Dlg.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/features2d.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/world.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIFDS04Dlg dialog



CIFDS04Dlg::CIFDS04Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IFDS04_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIFDS04Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIFDS04Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CIFDS04Dlg::OnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_CORNER, &CIFDS04Dlg::OnClickedButtonCorner)
	ON_BN_CLICKED(IDC_BUTTON_EDGE, &CIFDS04Dlg::OnClickedButtonEdge)
	ON_BN_CLICKED(IDC_BUTTON_BRISK, &CIFDS04Dlg::OnClickedButtonBrisk)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CIFDS04Dlg::OnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CIFDS04Dlg::OnClickedButtonClose)
END_MESSAGE_MAP()


// CIFDS04Dlg message handlers

BOOL CIFDS04Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIFDS04Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIFDS04Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIFDS04Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIFDS04Dlg::OnClickedButtonImport()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, _T(".jpg"), NULL, OFN_FILEMUSTEXIST, _T("Image Files|*.jpg;*.jpeg;*.png;*.bmp;*.tiff||"));
	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();
		ATL::CW2A tempStr(filePath.GetString());  // Convert CString to const char*
		std::string stdStr(tempStr);              // Construct a std::string

		cv::Mat img = cv::imread(stdStr);  // Use the std::string with cv::imread
		cv::Mat grayImg;
		cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);

		DisplayImage(IDC_STATIC_DISPLAY_LEFT, img, this);
		DisplayImage(IDC_STATIC_DISPLAY_RIGHT, grayImg, this);

		m_OriginalImage = img;
		m_GrayImage = grayImg;

		CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR);
		pProgress->SetPos(100); // Set progress bar to 100% upon load
	}

}

void CIFDS04Dlg::DisplayImage(int nID, const cv::Mat& img, CDialog* dlg)
{
	if (img.empty()) return;

	CStatic* pPicCtrl = (CStatic*)dlg->GetDlgItem(nID);
	if (!pPicCtrl) return;  // Safety check

	CRect rect;
	pPicCtrl->GetClientRect(&rect);
	cv::Size size(rect.Width(), rect.Height());  // Desired display size

	cv::Mat displayImg;
	cv::resize(img, displayImg, size); // Resize image to fit the display

	// Convert the image to a compatible format for CImage
	if (displayImg.channels() == 4) {
		cv::cvtColor(displayImg, displayImg, cv::COLOR_BGRA2BGR); // Assuming the source is BGRA
	}
	else if (displayImg.channels() == 3) {
		cv::cvtColor(displayImg, displayImg, cv::COLOR_BGR2BGRA); // Convert from BGR to BGRA
	}
	else if (displayImg.channels() == 1) {
		cv::cvtColor(displayImg, displayImg, cv::COLOR_GRAY2BGRA); // Convert grayscale to BGRA
	}

	// Ensure the image is continuous
	if (!displayImg.isContinuous()) {
		displayImg = displayImg.clone();
	}

	// Create a CImage to hold the display image
	CImage cImage;
	cImage.Create(size.width, size.height, 32);

	// Get the pointer to the bits of this CImage
	void* pDstBits = cImage.GetBits();
	int dstStep = cImage.GetPitch();
	unsigned char* pSrc = displayImg.data;

	// Copy the data
	for (int y = 0; y < displayImg.rows; ++y) {
		auto dstOffset = static_cast<size_t>(y) * static_cast<size_t>(dstStep);
		auto srcOffset = static_cast<size_t>(y) * static_cast<size_t>(displayImg.step);
		memcpy(reinterpret_cast<char*>(pDstBits) + dstOffset, pSrc + srcOffset, displayImg.cols * 4);
	}

	// Set the bitmap to the control
	pPicCtrl->SetBitmap(NULL); // Remove any previous bitmap
	HBITMAP hBitmap = (HBITMAP)cImage.Detach(); // Detach the bitmap to avoid cleanup issues
	pPicCtrl->SetBitmap(hBitmap); // The control now owns the bitmap
}

void CIFDS04Dlg::OnClickedButtonCorner()
{
	// TODO: Add your control notification handler code here
	if (m_GrayImage.empty())
		return;

	cv::Mat cornerStrength;
	cv::cornerHarris(m_GrayImage, cornerStrength, 2, 3, 0.04);

	// Normalize the result to 0 to 255
	cv::Mat cornerNorm, cornerImg;
	double minVal, maxVal;
	cv::minMaxLoc(cornerStrength, &minVal, &maxVal);
	cornerStrength.convertTo(cornerNorm, CV_32FC1, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
	cv::convertScaleAbs(cornerNorm, cornerImg);

	// Threshold and create a color image to show the corners
	cv::cvtColor(m_GrayImage, cornerImg, cv::COLOR_GRAY2BGR);  // Convert grayscale to BGR for visualization
	for (int j = 0; j < cornerNorm.rows; j++) {
		for (int i = 0; i < cornerNorm.cols; i++) {
			if ((int)cornerNorm.at<float>(j, i) > 200) {  // Threshold for detecting strong corners
				cv::circle(cornerImg, cv::Point(i, j), 5, cv::Scalar(0, 0, 255), 2);  // Draw in red
			}
		}
	}

	DisplayImage(IDC_STATIC_DISPLAY_RIGHT, cornerImg, this);
	UpdateProgressBar(100); // Assuming UpdateProgressBar sets the progress bar's position
}


void CIFDS04Dlg::OnClickedButtonEdge()
{
	// TODO: Add your control notification handler code here

	if (m_GrayImage.empty())
		return;

	// Apply a threshold or Canny edge detection to focus on objects rather than the frame edge
	cv::Mat processedImg;
	cv::Canny(m_GrayImage, processedImg, 50, 150); // Canny edge detection with thresholds

	// Find contours on the processed image
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(processedImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// Use the original grayscale image converted to color for displaying the contours
	cv::Mat contourImage;
	cv::cvtColor(m_GrayImage, contourImage, cv::COLOR_GRAY2BGR);  // Convert grayscale to BGR

	// Draw contours in blue
	for (const auto& contour : contours) {
		cv::drawContours(contourImage, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255, 0, 0), 2);
	}

	DisplayImage(IDC_STATIC_DISPLAY_RIGHT, contourImage, this);
	UpdateProgressBar(100); // Update the progress bar to 100% upon completion
}


void CIFDS04Dlg::OnClickedButtonBrisk()
{
	// TODO: Add your control notification handler code here
	if (m_GrayImage.empty())
		return;

	// Initialize BRISK detector
	auto brisk = cv::BRISK::create();
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;

	// Detect keypoints
	brisk->detect(m_GrayImage, keypoints);

	// Draw keypoints on the image
	cv::Mat briskImage;
	cv::drawKeypoints(m_GrayImage, keypoints, briskImage, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	DisplayImage(IDC_STATIC_DISPLAY_RIGHT, briskImage, this);
	UpdateProgressBar(100); // Update the progress bar to 100% upon completion
}


void CIFDS04Dlg::OnClickedButtonExport()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE, _T("*.jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("JPEG Files (*.jpg)|*.jpg||"));
	if (dlg.DoModal() == IDOK)
	{
		ATL::CW2A tempStr(dlg.GetPathName().GetString());  // Convert CString to const char*
		std::string stdStr(tempStr);                      // Construct a std::string

		cv::imwrite(stdStr, m_ModifiedImage);  // Use the std::string with cv::imwrite
	}
}

void CIFDS04Dlg::UpdateProgressBar(int value)
{
	CProgressCtrl* pProgressBar = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR);
	pProgressBar->SetPos(value);
}

void CIFDS04Dlg::OnClickedButtonClose()
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_CLOSE);
}
