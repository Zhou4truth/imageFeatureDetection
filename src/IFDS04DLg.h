
// IFDS04Dlg.h : header file
#include <opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/features2d.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/world.hpp>

#pragma once


// CIFDS04Dlg dialog
class CIFDS04Dlg : public CDialogEx
{
// Construction
public:
	CIFDS04Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IFDS04_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonImport();
	afx_msg void OnClickedButtonCorner();
	afx_msg void OnClickedButtonEdge();
	afx_msg void OnClickedButtonBrisk();
	afx_msg void OnClickedButtonExport();
	afx_msg void OnClickedButtonClose();
	void DisplayImage(int nID, const cv::Mat& img, CDialog* dlg);
	void UpdateProgressBar(int value);

private:
	cv::Mat m_OriginalImage;
	cv::Mat m_GrayImage;
	cv::Mat m_ModifiedImage;
	
};
