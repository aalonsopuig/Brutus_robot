// BrutusDlg.h : header file
// By Alejandro Alonso Puig
// February 2004
// mundobot.com
//
//

#if !defined(AFX_BRUTUSDLG_H__4E5EDD67_274B_11D8_B526_00104BF1F258__INCLUDED_)
#define AFX_BRUTUSDLG_H__4E5EDD67_274B_11D8_B526_00104BF1F258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "I2C.h"

/////////////////////////////////////////////////////////////////////////////
// CBrutusDlg dialog

class CBrutusDlg : public CDialog
{
// Construction
public:
	CBrutusDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBrutusDlg)
	enum { IDD = IDD_BRUTUS_DIALOG };
	int		m_Speed;
	int		m_Direc;
	int		m_Accel;
	int		m_CurrMD03_1;
	int		m_TempMD03_1;
	int		m_CurrSVD01;
	int		m_TempSVD01;
	BOOL	m_BumpLeft;
	BOOL	m_BumpRight;
	int		m_AccelSlider;
	int		m_YSonar;
	int		m_XSonar;
	int		m_DirecSlider;
	int		m_SpeedSlider;
	int		m_XSonarSlider;
	int		m_YSonarSlider;
	int		m_DistSonar;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrutusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private:
	unsigned char cSonarServoYLimit;
	unsigned char cSonarServoXLimit;
	int		YSonar;				//Values sent to sonar for positioning
	int		XSonar;				//Values sent to sonar for positioning
	I2c		Board;

	//I2C addresses for the different modules
	unsigned char cSRF08Add;		//I2C address SRF08 module (Sonar)
	unsigned char cSVI2CAdd;		//Address of the I2C Servo controller device
	unsigned char cMD03_1Add;		//Address of the MD03_1 module
	unsigned char cSVD01Add;		//Address of the MD03_1 module
	unsigned char cPORTAdd;			//Address of digital ports module

	//Sonar control functions
	int DistMeasured(void);				//Read distance value from Sonar
	void SonarPosition(int iX, int iY);	//Move Sonar to a desired position

	//Steering servo control functions
	unsigned char ReadCurrSVD01();			//Read SVD01 current
	unsigned char ReadTempSVD01();			//Read SVD01 Temperature
	void SetPosSVD01(unsigned char cPos);	//Move SVD01 to desired position

	//Motor driver control functions
	unsigned char ReadCurrMD03_1();				//Read MD03_1 current
	unsigned char ReadTempMD03_1();				//Read MD03_1 temperature
	void SetAccelMD03_1(unsigned char cAccel);	//Set MD03_1 acceleration
	void SetDirecMD03_1(unsigned char cDirec);	//Set MD03_1 run direction
	void SetSpeedMD03_1(unsigned char cSpeed);	//Set MD03_1 speed

	//Digital ports device control functions
	unsigned char cReadDPort();		//Read port from digital ports module


	unsigned char	cSpeedMD03_1,	//MD03_1 Speed value
					cAccelMD03_1,	//MD03_1 Acceleration value
					cDirecMD03_1,	//MD03_1 Direction value
					cTempMD03_1,	//MD03_1 Temperature value
					cCurrMD03_1;	//MD03_1 Current value

	unsigned char	cActPosSVD01,	//SVD01 Actual Position value
					cNewPosSVD01,	//SVD01 New Position value
					cTempSVD01,		//SVD01 Temperature value
					cCurrSVD01,		//SVD01 Current value
					cStatSVD01,		//SVD01 Status value
					cHDBSVD01,		//SVD01 HalfDeadBand value
					cVerSVD01;		//SVD01 Firmware version value

	CPoint	startpt, CPvalue;

	//Canvas limits
	int				iCanvDirLeft,
					iCanvDirRight,
					iCanvDirTop,
					iCanvDirBottom,
					iCanvSonLeft,
					iCanvSonRight,
					iCanvSonTop,
					iCanvSonBottom;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBrutusDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStop();
	afx_msg void OnUpdate();
	afx_msg void OnReleasedcaptureAccelSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawAccelSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawDirecSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSpeedSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawXSonarSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawYSonarSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkSonar();
	afx_msg void OnAuto1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRUTUSDLG_H__4E5EDD67_274B_11D8_B526_00104BF1F258__INCLUDED_)
