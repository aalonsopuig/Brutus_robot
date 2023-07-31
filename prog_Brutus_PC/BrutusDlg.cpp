// BrutusDlg.cpp : implementation file
// By Alejandro Alonso Puig
// February 2004
// mundobot.com
//

#include "stdafx.h"
#include "Brutus.h"
#include "BrutusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrutusDlg dialog

CBrutusDlg::CBrutusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrutusDlg::IDD, pParent)
{
	startpt=-1;
	CPvalue=-1;

	//Movement Canvas Limits
	iCanvDirLeft	=361;
	iCanvDirRight	=544;
	iCanvDirTop		=312;
	iCanvDirBottom	=475;

	//Sonar Canvas Limits
	iCanvSonLeft	=67;
	iCanvSonRight	=250;
	iCanvSonTop		=312;
	iCanvSonBottom	=475;

	//I2C Deviced addresses
	cSRF08Add	=224;		//I2C address SRF08 module
	cSVI2CAdd	=118;		//Address of the I2C Servo controller device
	cMD03_1Add	=176;		//I2C address MD03_1 module
	cSVD01Add	=120;		//I2C address SVD01 module
	cPORTAdd	=112;		//I2C address Digital Ports module

	cSonarServoXLimit=220;	//Servo limit for sonar
	cSonarServoYLimit=220;	//Servo limit for sonar
	YSonar = 128;
	XSonar = 128;

	//{{AFX_DATA_INIT(CBrutusDlg)
	m_Speed = 0;
	m_Direc = 128;
	m_Accel = 255;
	m_CurrMD03_1 = 0;
	m_TempMD03_1 = 0;
	m_CurrSVD01 = 0;
	m_TempSVD01 = 0;
	m_BumpLeft = FALSE;
	m_BumpRight = FALSE;
	m_AccelSlider = 0;		//Acceleration value for MD03 module
	m_YSonar = 90;
	m_XSonar = 90;
	m_DirecSlider = 50;
	m_SpeedSlider = 50;
	m_XSonarSlider = 50;
	m_YSonarSlider = 50;
	m_DistSonar = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBrutusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrutusDlg)
	DDX_Text(pDX, IDC_Speed, m_Speed);
	DDX_Text(pDX, IDC_Direc, m_Direc);
	DDX_Text(pDX, IDC_Accel, m_Accel);
	DDX_Text(pDX, IDC_CurrMD03_1, m_CurrMD03_1);
	DDX_Text(pDX, IDC_TempMD03_1, m_TempMD03_1);
	DDX_Text(pDX, IDC_CurrSVD01, m_CurrSVD01);
	DDX_Text(pDX, IDC_TempSVD01, m_TempSVD01);
	DDX_Check(pDX, IDC_BumpLeft, m_BumpLeft);
	DDX_Check(pDX, IDC_BumpRight, m_BumpRight);
	DDX_Slider(pDX, IDC_AccelSlider, m_AccelSlider);
	DDX_Text(pDX, IDC_YSonar, m_YSonar);
	DDX_Text(pDX, IDC_XSonar, m_XSonar);
	DDX_Slider(pDX, IDC_DirecSlider, m_DirecSlider);
	DDX_Slider(pDX, IDC_SpeedSlider, m_SpeedSlider);
	DDX_Slider(pDX, IDC_XSonarSlider, m_XSonarSlider);
	DDX_Slider(pDX, IDC_YSonarSlider, m_YSonarSlider);
	DDX_Text(pDX, IDC_DistSonar, m_DistSonar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBrutusDlg, CDialog)
	//{{AFX_MSG_MAP(CBrutusDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_AccelSlider, OnReleasedcaptureAccelSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_AccelSlider, OnCustomdrawAccelSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_DirecSlider, OnCustomdrawDirecSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SpeedSlider, OnCustomdrawSpeedSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_XSonarSlider, OnCustomdrawXSonarSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_YSonarSlider, OnCustomdrawYSonarSlider)
	ON_BN_CLICKED(IDC_ChkSonar, OnChkSonar)
	ON_BN_CLICKED(IDC_AUTO1, OnAuto1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrutusDlg message handlers

BOOL CBrutusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBrutusDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBrutusDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBrutusDlg::OnLButtonDown(UINT nFlags, CPoint point) 
//Check if clic on any of the canvas and update values
{
	//Clic over Movement Canvas
	if (point.x>=iCanvDirLeft && point.x<=iCanvDirRight &&		//Check Movement Canvas limits
		point.y>=iCanvDirTop && point.y<=iCanvDirBottom)
	{
		startpt.x=point.x;
		CPvalue.x=(point.x-iCanvDirLeft)*255/(iCanvDirRight-iCanvDirLeft);
		CPvalue.y=((iCanvDirTop+(iCanvDirBottom-iCanvDirTop)/2)-point.y)*240/((iCanvDirBottom-iCanvDirTop)/2);

		m_Direc=CPvalue.x;
		m_Speed=CPvalue.y;
		m_DirecSlider=(m_Direc)*100/255;
		m_SpeedSlider=(point.y-iCanvDirTop)*100/(iCanvDirBottom-iCanvDirTop);
		UpdateData(FALSE);						//Update values in form

		if (m_Speed>0)
		{
			cDirecMD03_1=2;						//Go to the front
			cSpeedMD03_1=CPvalue.y;				//At determined speed
		}	
		else 
		{
			if (m_Speed<0)
			{
				cDirecMD03_1=1;					//Go to the back
				cSpeedMD03_1=-CPvalue.y;		//At determined speed
			}
			else
			{
				cDirecMD03_1=0;					//Go to nowhere
				cSpeedMD03_1=0;					//At 0 speed
			}
		}

		cNewPosSVD01=m_Direc;
		SetPosSVD01 (cNewPosSVD01);				//Move steering servo
		Sleep (10);

		SetDirecMD03_1 (cDirecMD03_1);			//Send command to MD03
		SetSpeedMD03_1 (cSpeedMD03_1);			//Send command to MD03

	}

	//Clic over Sonar Canvas
	if (point.x>=iCanvSonLeft && point.x<=iCanvSonRight &&		//Check Sonar Canvas limits
		point.y>=iCanvSonTop && point.y<=iCanvSonBottom)
	{
		startpt.y=point.y;
		CPvalue.x=(point.x-iCanvSonLeft)*cSonarServoXLimit/(iCanvSonRight-iCanvSonLeft);
		CPvalue.y=(iCanvSonBottom-point.y)*cSonarServoYLimit/(iCanvSonBottom-iCanvSonTop);

		XSonar=CPvalue.x;
		YSonar=CPvalue.y;
		m_XSonarSlider=XSonar*100/cSonarServoXLimit;
		m_YSonarSlider=100-YSonar*100/cSonarServoYLimit;
		SonarPosition (XSonar, YSonar);				//Move sonar to specified position
		m_DistSonar=DistMeasured();					//Measure distance from sonar
		m_XSonar=XSonar*180/cSonarServoXLimit;		//Convert values to degrees		
		m_YSonar=YSonar*180/cSonarServoYLimit;		//Convert values to degrees	

		UpdateData(FALSE);							//Update form
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CBrutusDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	startpt=-1;
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CBrutusDlg::OnMouseMove(UINT nFlags, CPoint point) 
//Manage Sonar canvas and Movement canvas
{


	//Mouse move over Movement Canvas
	if (startpt.x != -1 &&
		point.x>=iCanvDirLeft && point.x<=iCanvDirRight &&		//Check Movement Canvas limits
		point.y>=iCanvDirTop && point.y<=iCanvDirBottom)
	{
		CPvalue.x=(point.x-iCanvDirLeft)*255/(iCanvDirRight-iCanvDirLeft);
		CPvalue.y=((iCanvDirTop+(iCanvDirBottom-iCanvDirTop)/2)-point.y)*240/((iCanvDirBottom-iCanvDirTop)/2);

		m_Direc=CPvalue.x;
		m_Speed=CPvalue.y;
		m_DirecSlider=(m_Direc)*100/255;
		m_SpeedSlider=(point.y-iCanvDirTop)*100/(iCanvDirBottom-iCanvDirTop);
		UpdateData(FALSE);						//Update values in form

		if (m_Speed>0)
		{
			cDirecMD03_1=2;						//Go to the front
			cSpeedMD03_1=CPvalue.y;				//At determined speed
		}	
		else 
		{
			if (m_Speed<0)
			{
				cDirecMD03_1=1;					//Go to the back
				cSpeedMD03_1=-CPvalue.y;		//At determined speed
			}
			else
			{
				cDirecMD03_1=0;					//Go to nowhere
				cSpeedMD03_1=0;					//At 0 speed
			}
		}

		cNewPosSVD01=m_Direc;
		SetPosSVD01 (cNewPosSVD01);				//Move steering servo
		Sleep (10);

		SetDirecMD03_1 (cDirecMD03_1);			//Send command to MD03
		SetSpeedMD03_1 (cSpeedMD03_1);			//Send command to MD03

		CDialog::OnMouseMove(nFlags, point);
	}

	//Mouse move over Sonar Canvas
	if (startpt.y != -1 &&
		point.x>=iCanvSonLeft && point.x<=iCanvSonRight &&		//Check Sonar Canvas limits
		point.y>=iCanvSonTop && point.y<=iCanvSonBottom)
	{
		CPvalue.x=(point.x-iCanvSonLeft)*cSonarServoXLimit/(iCanvSonRight-iCanvSonLeft);
		CPvalue.y=(iCanvSonBottom-point.y)*cSonarServoYLimit/(iCanvSonBottom-iCanvSonTop);

		XSonar=CPvalue.x;
		YSonar=CPvalue.y;
		m_XSonarSlider=XSonar*100/cSonarServoXLimit;
		m_YSonarSlider=100-YSonar*100/cSonarServoYLimit;

		SonarPosition (XSonar, YSonar);				//Move sonar to specified position
		m_DistSonar=DistMeasured();					//Measure distance from sonar
		m_XSonar=XSonar*180/cSonarServoXLimit;		//Convert values to degrees		
		m_YSonar=YSonar*180/cSonarServoYLimit;		//Convert values to degrees	

		UpdateData(FALSE);							//Update form

		CDialog::OnMouseMove(nFlags, point);
	}

	
}

void CBrutusDlg::SetSpeedMD03_1(unsigned char cSpeed)
//Set Speed on MD03_1 module
{
	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cMD03_1Add);		//Slave address
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(2);				//Registry (Speed)
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(cSpeed);			//Speed
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
}

void CBrutusDlg::SetDirecMD03_1(unsigned char cDirec)
//Set Direction value on MD03_1 module
{
	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cMD03_1Add);		//Slave address
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(0);				//Registry (Direction)
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(cDirec);			//Direction
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
}

void CBrutusDlg::SetAccelMD03_1(unsigned char cAccel)
//Set Acceleration value on MD03_1 module
{
	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cMD03_1Add);		//Slave address
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(3);				//Registry (Acceleration)
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(cAccel);			//Acceleration
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
}

void CBrutusDlg::OnStop() 
//Stop motors (MD03)
{
	SetDirecMD03_1 (0);						//Send command to MD03
	SetSpeedMD03_1 (0);						//Send command to MD03
	m_Speed			=0;
	m_SpeedSlider	= 50;
	UpdateData(FALSE);						//Update Form	
}




unsigned char CBrutusDlg::ReadTempMD03_1()
//Read Temperature value on MD03_1 module
{
	unsigned char cTemp;

	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cMD03_1Add);		//Slave address
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(4);				//Registry (Temperature)
	Board.GenerateAcknowledge();
	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cMD03_1Add+1);	//Slave address (read mode)
	Board.GenerateAcknowledge();
	cTemp=Board.InputByteFromI2C();			//Take value from slave
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
	return (cTemp);
}

unsigned char CBrutusDlg::ReadCurrMD03_1()
//Read Current value on MD03_1 module
{
	unsigned char cCurr;

	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cMD03_1Add);		//Slave address
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(5);				//Registry (Current)
	Board.GenerateAcknowledge();
	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cMD03_1Add+1);	//Slave address (read mode)
	Board.GenerateAcknowledge();
	cCurr=Board.InputByteFromI2C();			//Take value from slave
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
	return (cCurr);

}

void CBrutusDlg::OnUpdate() 
//Update button has been pressed. Some values are updated at screen
{
	unsigned char cDPort;
	cDPort=cReadDPort();					//Read Digital port
	m_BumpRight=m_BumpLeft=FALSE; 
	if (cDPort==1) m_BumpLeft=TRUE;
	if (cDPort==2) m_BumpRight=TRUE;
	if (cDPort==3) m_BumpLeft=m_BumpRight=TRUE;
	m_TempSVD01=ReadTempSVD01();			//Read Temperature from SVD01
	m_CurrSVD01=ReadCurrSVD01();			//Read Current from SVD01
	m_TempMD03_1=ReadTempMD03_1();			//Read Temperature from MD03_1
	m_CurrMD03_1=ReadCurrMD03_1();			//Read Current from MD03_1
	UpdateData(FALSE);						//Update Form
	
}

void CBrutusDlg::SetPosSVD01(unsigned char cPos)
// Establish position of SVD01 
{
	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cSVD01Add);		//Slave address
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(3);				//Registry (Position)
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(255-cPos);		//Position
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
}

unsigned char CBrutusDlg::ReadTempSVD01()
//Read Temperature value on SVD01 module
{
	unsigned char cTemp;

	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cSVD01Add);		//Slave address
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(4);				//Registry (Temperature)
	Board.GenerateAcknowledge();
	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cSVD01Add+1);		//Slave address (read mode)
	Board.GenerateAcknowledge();
	cTemp=Board.InputByteFromI2C();			//Take value from slave
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
	return (cTemp);

}

unsigned char CBrutusDlg::ReadCurrSVD01()
//Read Current value on SVD01 module
{
	unsigned char cCurr;

	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cSVD01Add);		//Slave address
	Board.GenerateAcknowledge();
	Board.OutputByteToI2C(5);				//Registry (Current)
	Board.GenerateAcknowledge();
	Board.GenerateStartCondition();	
	Board.OutputByteToI2C(cSVD01Add+1);		//Slave address (read mode)
	Board.GenerateAcknowledge();
	cCurr=Board.InputByteFromI2C();			//Take value from slave
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
	return (cCurr);

}



unsigned char CBrutusDlg::cReadDPort()
//Read the port from the Digital Port module
{
	unsigned char cValue;

	Board.GenerateStartCondition();

	//Serial output of Chipcode for readmode
	Board.OutputByteToI2C(cPORTAdd+1);

	Board.GenerateAcknowledge();

	//Serial input of ChipData
	cValue = Board.InputByteFromI2C();

	Board.GenerateAcknowledge();

	Board.GenerateStopCondition();

	return(cValue);
}



void CBrutusDlg::OnReleasedcaptureAccelSlider(NMHDR* pNMHDR, LRESULT* pResult) 
//When Accelerator slider is relesed. The Value is sent to the device
//and updated at the screen form
{
	UpdateData(TRUE);						//take variables from Form
	m_Accel	=255-(255*m_AccelSlider/100);
	UpdateData(FALSE);						//Update Form
	SetAccelMD03_1(255-m_Accel);				//Send command to MD03

	*pResult = 0;
}

void CBrutusDlg::OnCustomdrawAccelSlider(NMHDR* pNMHDR, LRESULT* pResult) 
//When Accelerator slider is moved. The Value is updated at the screen form only
{
	UpdateData(TRUE);						//take variables from Form
	m_Accel	=255-(255*m_AccelSlider/100);
	UpdateData(FALSE);						//Update Form
	
	*pResult = 0;
}



void CBrutusDlg::OnCustomdrawDirecSlider(NMHDR* pNMHDR, LRESULT* pResult) 
//Control movement of Direction Slider (Steering)
{
	UpdateData(TRUE);						//take variables from Form
	m_Direc	=255*m_DirecSlider/100;
	UpdateData(FALSE);						//Update Form

	cNewPosSVD01=m_Direc;
	SetPosSVD01 (cNewPosSVD01);				//Move steering servo
	Sleep (10);

	*pResult = 0;
}



void CBrutusDlg::OnCustomdrawSpeedSlider(NMHDR* pNMHDR, LRESULT* pResult) 
//Control movement of Speed Slider
{
	UpdateData(TRUE);						//take variables from Form
	m_Speed	=241-m_SpeedSlider*4.82;
	UpdateData(FALSE);						//Update Form

	if (m_Speed>0)
	{
		cDirecMD03_1=2;						//Go to the front
		cSpeedMD03_1=m_Speed;				//At determined speed
	}
	else 
	{
		if (m_Speed<0)
		{
			cDirecMD03_1=1;					//Go to the back
			cSpeedMD03_1=-m_Speed;			//At determined speed
		}
		else
		{
			cDirecMD03_1=0;					//Go to nowhere
			cSpeedMD03_1=0;					//At 0 speed
		}
	}

	SetDirecMD03_1 (cDirecMD03_1);			//Send command to MD03
	SetSpeedMD03_1 (cSpeedMD03_1);			//Send command to MD03

	*pResult = 0;
}

void CBrutusDlg::OnCustomdrawXSonarSlider(NMHDR* pNMHDR, LRESULT* pResult) 
//Control X axis Sonar movement slider
{
	UpdateData(TRUE);								//take variables from Form
	XSonar=m_XSonarSlider*cSonarServoXLimit/100;	//Obtain value in range 0-220
	SonarPosition (XSonar, YSonar);					//Move sonar to specified position
	m_DistSonar=DistMeasured();						//Measure distance from sonar
	m_XSonar=XSonar*180/cSonarServoXLimit;			//Obtain value in degrees
	UpdateData(FALSE);								//Update form

	*pResult = 0;
}

void CBrutusDlg::OnCustomdrawYSonarSlider(NMHDR* pNMHDR, LRESULT* pResult) 
//Control Y axis Sonar movement slider
{
	UpdateData(TRUE);									//take variables from Form
	YSonar=(100-m_YSonarSlider)*cSonarServoYLimit/100;	//Obtain value in range 0-220
	SonarPosition (XSonar, YSonar);					//Move sonar to specified position
	m_DistSonar=DistMeasured();							//Measure distance from sonar
	m_YSonar=YSonar*180/cSonarServoYLimit;			//Obtain value in degrees
	UpdateData(FALSE);									//Update form
	
	*pResult = 0;
}

int CBrutusDlg::DistMeasured(void)
//Read distance in cm from Sonar
{
	int	iDistH=0, iDistL =0;				//Distance measured


		//Request to make a measure
		Board.GenerateStartCondition();
		Board.OutputByteToI2C(cSRF08Add);	//Slave address
		Board.GenerateAcknowledge();
		Board.OutputByteToI2C(0);			//Registry
		Board.GenerateAcknowledge();
		Board.OutputByteToI2C(81);			//Command to measure in cm
		Board.GenerateAcknowledge();
		Board.GenerateStopCondition();

		Sleep (100);						//Pause to give time to read
	
		//Request the value measured
		Board.GenerateStartCondition();
		Board.OutputByteToI2C(cSRF08Add);	//Slave address
		Board.GenerateAcknowledge();
		Board.OutputByteToI2C(2);			//Registry
		Board.GenerateAcknowledge();

		Board.GenerateStartCondition();
		Board.OutputByteToI2C(cSRF08Add+1);	//Slave address (read mode)
		Board.GenerateAcknowledge();
		iDistH=Board.InputByteFromI2C();	//Take H value of measurement
		Board.GenerateAcknowledge();
		Board.GenerateStopCondition();

		Board.GenerateStartCondition();
		Board.OutputByteToI2C(cSRF08Add);	//Slave address
		Board.GenerateAcknowledge();
		Board.OutputByteToI2C(3);			//Registry
		Board.GenerateAcknowledge();

		Board.GenerateStartCondition();
		Board.OutputByteToI2C(cSRF08Add+1);	//Slave address (read mode)
		Board.GenerateAcknowledge();
		iDistL=Board.InputByteFromI2C();	//Take L value of measurement
		Board.GenerateAcknowledge();
		Board.GenerateStopCondition();

		return(iDistH*256+iDistL);

}

void CBrutusDlg::SonarPosition(int iX, int iY)
//Move sonar to desired position
{
	char	cServoX=0,cServoY=1;			//Servo identifications

	//Control Servos Limits
	if (iX>cSonarServoXLimit) iX=cSonarServoXLimit;
	if (iY>cSonarServoYLimit) iY=cSonarServoYLimit;
	if (iX<0) iX=0;
	if (iY<0) iY=0;


	//ServoX
	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cSVI2CAdd);				//Slave address
	Board.GenerateAcknowledge();
	Sleep (10);
	Board.OutputByteToI2C(3);						//Registry (Position)
	Board.GenerateAcknowledge();
	Sleep (10);
	Board.OutputByteToI2C(cServoX);					//Device 
	Board.GenerateAcknowledge();
	Sleep (10);
	Board.OutputByteToI2C(cSonarServoXLimit-iX);	//Value	
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
	Sleep (10);


	//ServoY
	Board.GenerateStartCondition();
	Board.OutputByteToI2C(cSVI2CAdd);		//Slave address
	Board.GenerateAcknowledge();
	Sleep (10);
	Board.OutputByteToI2C(3);				//Registry (Position)
	Board.GenerateAcknowledge();
	Sleep (10);
	Board.OutputByteToI2C(cServoY);			//Device 
	Board.GenerateAcknowledge();
	Sleep (10);
	Board.OutputByteToI2C(iY);				//Value	
	Board.GenerateAcknowledge();
	Board.GenerateStopCondition();
	Sleep (10);

}

void CBrutusDlg::OnChkSonar() 
//Check distance button clic
{
	m_DistSonar=DistMeasured();							//Measure distance from sonar
	UpdateData(FALSE);									//Update form
	
}

void CBrutusDlg::OnAuto1() 
//Run the autonomous control process
{
	int	iTimeout1, iTimeout2=200, iTimeout3=1000, iTimeout4=2000, iTimeout5=200;
		
	unsigned char cDPort, cDistSonar;


	//State-1. Go ahead until iTimeout1
	for (iTimeout1=0; iTimeout1<=100; iTimeout1++)
	{
		SonarPosition(128, 128);					//Sonar looking to the front
		SetPosSVD01(128);							//Steering centered
		SetAccelMD03_1(128);						//Establish a medium acceleration value for motors
		SetDirecMD03_1(2);							//Wheels rotation direction: Move robot to the front
		SetSpeedMD03_1(50);							//Moderate speed to the motors, so could react properly
		cDistSonar=DistMeasured();					//Measure distance from sonar
		cDPort=cReadDPort();						//Read Digital port (Check bumpers)
		
		if (((cDistSonar!=0) && (cDistSonar<50)) ||	//Obstacle closer than 50cm
			(cDPort!=0) && (cDPort<=3))							//Or bumpers detected it
		{
			//State-2.Stop until iTimeout2
			SetDirecMD03_1(0);						//Wheels stopped
			Sleep (iTimeout2);						//Time to really stop

			//State-3.Go back until iTimeout3
			SetDirecMD03_1(1);						//Wheels rotation direction: Move robot back
			Sleep (iTimeout3);						//go back during a while

			//State-4. Go back turning until iTimeout4
			SetPosSVD01(0);							//Steering turned to one side
			Sleep (iTimeout4);						//turn during a while

			//State-5.Stop until iTimeout5
			SetDirecMD03_1(0);						//Wheels stopped
			Sleep (iTimeout5);						//Time to really stop
		}
	}

	
}
