//MENU----------------------
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
{
	image_But_OFF=image_But_OFF->FromFile("1.bmp");//(gcnew System::Windows::Forms::Image^)
	image_But_ON=image_But_ON->FromFile("2.bmp");//(gcnew System::Windows::Forms::Image^)
	
	this->P_INF->Location = System::Drawing::Point(5, 75);
	this->P_SEARCH->Location = System::Drawing::Point(5, 73);
	LoadRobotMenu();
}
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
{
	 LoadRobotMenu();
}
private: void OFF(System::String^ mess)
{
	L_Status->Text = mess;
	this->Enabled=false;
}
private: void ON()
{
	this->Enabled = true;
	L_Status->Text =L"Status";
	LoadRobotMenu();
}		 
private: void ON(System::String^ mess)
{
	this->Enabled = true;
	L_Status->Text =mess;
	LoadRobotMenu();
}
private: void LoadRobotMenu()
{
	C_Dri->Checked=(R[RobActNom].port!=0);
	C_Con->Checked=(R[RobActNom].connected);
	if (L_Status->Text ==L"Status")
	{
		if (R[RobActNom].port)
			if (R[RobActNom].connected)
				L_Status->Text ="Device is connected.";
			else
				L_Status->Text ="Device is ready.";
		else
			L_Status->Text ="Device is unready.";
	
		if (R[RobActNom].installing) 
		{
			if (R[RobActNom].ECOMSumm())
			{	
				L_Status->Text ="Driver installed.";
			}
			else
			{
				L_Status->Text ="Installing Driver...";
			}
		}
	}
	if (PipeConnect) pictureBox1->Image=image_But_ON;
	else pictureBox1->Image=image_But_OFF;
	
	if (RobActNom>1) B_RobPrev->Enabled=true;
	else B_RobPrev->Enabled=false;
	if (RobActNom<MAX_DEVICE) B_RobNext->Enabled=true;
	else B_RobNext->Enabled=false;

	SetText(label6,Lettons[RobActNom].Nlet);
	SetText(L_RobNom,RobActNom);
	
	if (T_port->BorderStyle==System::Windows::Forms::BorderStyle::FixedSingle)	
		SetText(T_port,R[RobActNom].port); 
	
	if (T_addr->BorderStyle==System::Windows::Forms::BorderStyle::FixedSingle)
		SetText(T_addr,R[RobActNom].ReBTaddr());

	if (R[RobActNom].ProgName) 
		SetText(T_PROG,R[RobActNom].ProgName);
	else
		SetText(T_PROG,"");
	
	
	//C_Aut->Checked=R[RobActNom].authenticated;
	//{
	//	B_Enable->Enabled=R[RobActNom].authenticated;
	//	B_Disable->Enabled=R[RobActNom].authenticated;
	//}
	{	
		B_Dis->Enabled=((R[RobActNom].port)||(R[RobActNom].connected));
		B_Con->Enabled=((R[RobActNom].port)||(R[RobActNom].connected));
		B_Clean->Enabled=((R[RobActNom].port)||(R[RobActNom].connected));
	}
} 

//next-prev----------------------------------------------
private: System::Void B_RobPrev_Click(System::Object^  sender, System::EventArgs^  e) 
{
	RobActNom+=-1;
	this->L_Status->Text = L"Status";
	LoadRobotMenu();
}
private: System::Void B_RobNext_Click(System::Object^  sender, System::EventArgs^  e) 
{
	RobActNom+=1;
	this->L_Status->Text = L"Status";
	LoadRobotMenu();
}

//Authenticate----------------------
private: System::Void B_Aut_Click(System::Object^  sender, System::EventArgs^  e) 
{
	OFF("Authenticating...");
	if(!R[RobActNom].Authenticating())
	{
		ON("Error! Checked device");
	}
	else
	{
		OFF("Authenticating OK, Instal Driver...");
	}

	if (!R[RobActNom].SearchRegCOM())
	{
		if (R[RobActNom].InstalDriver())//Enable_ReCom();
		{
			ON("Instaling Driver...");
		}
		else 
		{
			ON("Error installing params");
		}
	}
	else
	{
		ON("Device is ready.");
	}
}

//Connected------
private: System::Void B_Con_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 OFF("Connecting...");
			 if (R[RobActNom].SgConnect()) 
			 {
				 ON("Connected OK!");
			 }
			 else
			 {
				 ON("Connected Error! Checked device");
			 }
			 
		 }
private: System::Void B_Dis_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 OFF("Disconnecting...");
			 R[RobActNom].SgClose();
			 ON("Disconnect.");
		 } 
private: System::Void B_Send_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 OFF("Sending...");
			 SaveMes(SysStringToChar(T_Send->Text),RobActNom);
			 //R[RobActNom].SgSend(SysStringToChar(T_Send->Text));
			 ON("Sending is successfully");
		 }
private: System::Void B_Clean_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 OFF("Clean Connect...");
			 R[RobActNom].SgClean();
			 ON("Clean");
		 }		

//SEARCH
void SEARCH_VizAddr()
{
	checkedListBox1->Items->Clear();
	char addr[18];
	for(int i=0; i<MyBTdevice.m_device_id; i++)
	{
		ba2str(MyBTdevice.SaveAddr[i],addr);
		System::String^ Saddr=L"";
		System::String^ Sname=L"";
		
		for(int j=0; (MyBTdevice.SaveName[i][j]!=0)&&(j<248); j++)
		{
			Sname+=wchar_t(MyBTdevice.SaveName[i][j]);
		}
		for(int j=0;(addr[j]!=0)&&(j<18);j++)
		{
			Saddr+=wchar_t(addr[j]);
		}
		checkedListBox1->Items->Add(Saddr+L" - "+Sname);//label1->Text);
		delete(Saddr);
		delete(Sname);
	}
}
private: System::Void B_SEARCH_Click(System::Object^  sender, System::EventArgs^  e) 
{
	B_RESEARCH->Enabled=true;
	Searching=1;
	P_SEARCH->Visible = true;
	P_SEARCH->Enabled = true;
	if (!MyBTdevice.m_device_id) 
		B_RESEARCH_Click(sender,e);
	else 
		SEARCH_VizAddr();
}
private: System::Void B_Search_Cancel_Click(System::Object^  sender, System::EventArgs^  e) 
{
	P_SEARCH->Visible = false;
	P_SEARCH->Enabled = false;
	Searching=0;
	LoadRobotMenu();
}	 
private: System::Void checkedListBox1_ItemCheck(System::Object^  sender, System::Windows::Forms::ItemCheckEventArgs^  e) 
{
	if (Searching==1)
	{
		R[RobActNom].m_device_info.Address=MyBTdevice.SaveAddr[e->Index];
		R[RobActNom].GetInfo();
	}
	if (Searching==2)
	{
		R[RobActNom].port=MyBTdevice.COMs[e->Index];
	}
	B_Search_Cancel_Click(sender,e);
	ON();
}
private: System::Void B_COM_SEARCH_Click(System::Object^  sender, System::EventArgs^  e) 
{ 
	B_RESEARCH->Enabled=false;
	Searching=2;
	{
		OFF("Scan DRIVERS...");
		MyBTdevice.Find_COM_Save();
		ON("Select COM port");
	}
	checkedListBox1->Items->Clear();
	P_SEARCH->Visible = true;
	P_SEARCH->Enabled = true;			
	int com;
	for(int i=0; i<MyBTdevice.COM_id; i++)
	{
		com=MyBTdevice.COMs[i];
		checkedListBox1->Items->Add(IntToSysString(com));
	}
}
private: System::Void B_RESEARCH_Click(System::Object^  sender, System::EventArgs^  e) 
{
	OFF("Scan device...");
	MyBTdevice.Find_Device_Save();
	SEARCH_VizAddr();
	ON("Select device");
}

//FILE-----------------
private: System::Void B_PROG_DOWNLOAD_Click(System::Object^  sender, System::EventArgs^  e) 
{
	OFF("Downloading programm for NXT");
	if (!R[RobActNom].DownloadProg())
	{
		if (!R[RobActNom].DownloadProg2())
			ON("Programm successfully download");
		ON("Downloading ERROR");
	}
	else
	{
		ON("Programm successfully download");
	}
}
private: System::Void B_PROG_SEARCH_Click(System::Object^  sender, System::EventArgs^  e) 
{
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
	if (R[RobActNom].ProgAddr)
	{
		openFileDialog1->InitialDirectory = CharToSysString(R[RobActNom].ProgAddr);
	}
	openFileDialog1->Filter = "All files|*|NXTosek files (*.rxe)|*.rxe";
	openFileDialog1->FilterIndex = 2;
	openFileDialog1->RestoreDirectory = true;

	if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )
	{
		char * SProgAddr=SysStringToChar(openFileDialog1->ToString());
		int L=strlen(SProgAddr);
		int L1=L-56;
		int L2=0;
		while((L2<L1)&&(SProgAddr[L-L2-1]!='\\'))
			L2++;
		if (R[RobActNom].ProgAddr) 
		delete(R[RobActNom].ProgAddr);
		R[RobActNom].ProgAddr=new char[L1+1];
		for(int i=0; i<=L1; i++)
			R[RobActNom].ProgAddr[i]=SProgAddr[L-L1+i];
		if (R[RobActNom].ProgName) 
			delete(R[RobActNom].ProgName);
		R[RobActNom].ProgName=new char[L2+1];
		for(int i=0; i<=L2; i++)
			R[RobActNom].ProgName[i]=SProgAddr[L-L2+i];
		printf("Set NXTrun programm by device #%d\n%s\n%s\n",RobActNom,R[RobActNom].ProgAddr,R[RobActNom].ProgName);
	}
}	 
private: System::Void B_Run_Click(System::Object^  sender, System::EventArgs^  e) 
{
	OFF("Run programm for device...");
	R[RobActNom].RunProgramm();
	ON();
}//---------------------///

//OLD-------------
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 { 
			 OFF("Instal Driver...");
			 if (R[RobActNom].InstalDriver())
			 {
				 if (R[RobActNom].port)
					 ON("Driver is founded.");
				 else
					 ON("Instaling Driver...");
			 }
			 else 
			 {
				 ON("Error params");
			 }
		 }
private: System::Void B_Disable_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 OFF("Uninstall Driver...");
			 R[RobActNom].Disable();
			 ON();
		 }
private: System::Void B_COMReg_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 OFF("Search Reestr");
			 if (! R[RobActNom].SearchRegCOM())
			 {
				 ON("NO INSTALLED DRIVER");
			 }
			 else
			 {
				 ON("Device is ready.");
			 }
		 }
private: System::Void T_port_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
{
	if ((int)e->KeyData== 13)
	{
		bool re=true;
		int port=0;
		if (SysStringToChar(T_port->Text)[0]!=0)
		{
			char * a=SysStringToChar(T_port->Text);
			int i = 0;
			while ((re)&&(a[i]))
			{
				if ((a[i]<48)||(a[i]>57)) re=false;
				port=port*10+a[i]-48;
				i++;
			}
		}
		if (re) 
		{
			int ports=R[RobActNom].port;
			R[RobActNom].port=port;
			if (port==0) re=1;
			else
				re=R[RobActNom].COMtest();
			
			if (re) 
			{
				printf("FORM ENDING_ Device #%d .port= %d\n",RobActNom,port);
				ON("New COMport adopted");
			}
			else
			{	
				ON("Not activited port!");
				R[RobActNom].port=ports;
			}
		}
		else 
		{
			ON("Not corrected port!");
		}
		T_port->BorderStyle=System::Windows::Forms::BorderStyle::FixedSingle;
	}
	else
	{
		T_port->BorderStyle=System::Windows::Forms::BorderStyle::None;
	}
	//printf(".");
}
private: System::Void T_addr_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
{
	if ((int)e->KeyData== 13)
	{
		bool re=true;
		char BTaddr[18]="00:00:00:00:00:00";
		if (SysStringToChar(T_addr->Text)[0]!=0)
		{
			char * a=SysStringToChar(T_addr->Text);
			int i = 0;
			while ((re)&&(a[i]))
			{
				if (!(
						((a[i]==':')&&(!((i+1)%3)))||
						(
							((i+1)%3)&&
							(
								((a[i]>='0')&&(a[i]<='9'))||
								((a[i]>='A')&&(a[i]<='F'))
							)
						)
					)) re=false;
				BTaddr[i]=a[i];
				i++;
			}
		}
		if (re)
		{
			R[RobActNom].SetBTaddr(BTaddr);
			printf("FORM ENDING_ Device #%d .addr= %s\n",RobActNom,BTaddr);
			ON("New BTaddres adopted");
		}
		else
		{
			ON("Not corrected BTaddres");
		}
		T_addr->BorderStyle=System::Windows::Forms::BorderStyle::FixedSingle;
	}
	else
	{
	T_addr->BorderStyle=System::Windows::Forms::BorderStyle::None;
	}
}