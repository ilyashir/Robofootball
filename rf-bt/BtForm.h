#pragma once

#define TimeForMess 2000

#include <stdio.h>
#include <iostream>
bool ShowConsole=false;
int RobActNom = 1;
int COLOR=1;
long MessTime; 
int Searching=0; //1 - BTADRR 2-COM PORT
/*
char * SysStringToChar(System::String^ string) 
{
return (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(string);
}*/

System::String^ CharToSysString(char* ch) 
{ 
	char * chr=new char[]=ch;
	System::String^ str;
	for(int i=0;chr[i]!='\0';i++)
	{
	         str+=wchar_t(ch[i]);
	}
	return str;
}
System::String^ IntToSysString(int d)
{
	char ch[7];
	itoa (d,ch,10);
	return CharToSysString(ch);
}
void SetText(System::Windows::Forms::Label^ unit ,int i)
{
	System::String^ str;
	if((i/100)%10) str+=wchar_t(48+(i/100)%10);
	if(((i/100)%10)||((i/10)%10)) str+=wchar_t(48+(i/10)%10);
	str+=wchar_t(48+(i)%10);
	unit->Text=str;
	delete(str);
}
void SetText(System::Windows::Forms::TextBox^ unit ,int i)
{
	System::String^ str;
	if((i/100)%10) str+=wchar_t(48+(i/100)%10);
	if(((i/100)%10)||((i/10)%10)) str+=wchar_t(48+(i/10)%10);
	str+=wchar_t(48+(i)%10);
	unit->Text=str;
	delete(str);
}
void SetText(System::Windows::Forms::Label^ unit ,char * ch)
{
	System::String^ str;
	for(int i=0;ch[i]!=0;i++)
	{
         str+=wchar_t(ch[i]);
	}
	unit->Text=str;
	delete(str);
}
void SetText(System::Windows::Forms::TextBox^ unit ,char * ch)
{
	System::String^ str;
	for(int i=0;ch[i]!=0;i++)
	{
         str+=wchar_t(ch[i]);
	}
	unit->Text=str;
	delete(str);
}

namespace FORM {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//(cli::safe_cast<System::ComponentModel::ISupportIn itialize^[color=blue]>(this->pictureBox1))->BeginInit();[/color]

			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}


	protected: 

	private: System::Windows::Forms::Label^  L_RobNom;
	private: System::Windows::Forms::CheckBox^  C_Con;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  T_addr;
	private: System::Windows::Forms::Button^  B_Con;
	private: System::Windows::Forms::Button^  B_Dis;
	private: System::Windows::Forms::Button^  B_Send;
	private: System::Windows::Forms::TextBox^  T_Send;
	private: System::Windows::Forms::Button^  B_Clean;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Timer^  timer1;
			 
			 Image^ image_But_OFF;
			 Image^ image_But_ON;

	private: System::Windows::Forms::Button^  B_SEARCH;
	private: System::Windows::Forms::CheckedListBox^  checkedListBox1;
	private: System::Windows::Forms::CheckBox^  C_Dri;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  B_Disable;
	private: System::Windows::Forms::Button^  B_Enable;
	private: System::Windows::Forms::CheckBox^  C_Aut;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  T_port;
	private: System::Windows::Forms::Button^  B_RobPrev;
	private: System::Windows::Forms::Button^  B_RobNext;
	private: System::Windows::Forms::Button^  B_Run;
	private: System::Windows::Forms::Button^  B_Aut;
	private: System::Windows::Forms::Panel^  P_INF;
	private: System::Windows::Forms::Label^  L_INF;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Panel^  P_SEARCH;

	private: System::Windows::Forms::Button^  B_Search_Cancel;
	private: System::Windows::Forms::Button^  B_RESEARCH;
	private: System::Windows::Forms::Button^  B_COM_SEARCH;
	private: System::Windows::Forms::TextBox^  T_PROG;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  button2;

	private: System::Windows::Forms::Button^  B_PROG_SEARCH;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  B_PROG_DOWNLOAD;
	private: System::Windows::Forms::Button^  B_COMReg;
	private: System::Windows::Forms::Label^  L_Status;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Button^  B_REM;





	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Требуется переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Обязательный метод для поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->L_RobNom = (gcnew System::Windows::Forms::Label());
			this->C_Con = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->T_addr = (gcnew System::Windows::Forms::TextBox());
			this->B_Con = (gcnew System::Windows::Forms::Button());
			this->B_Dis = (gcnew System::Windows::Forms::Button());
			this->B_Send = (gcnew System::Windows::Forms::Button());
			this->T_Send = (gcnew System::Windows::Forms::TextBox());
			this->B_Clean = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->B_SEARCH = (gcnew System::Windows::Forms::Button());
			this->checkedListBox1 = (gcnew System::Windows::Forms::CheckedListBox());
			this->C_Dri = (gcnew System::Windows::Forms::CheckBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->B_Disable = (gcnew System::Windows::Forms::Button());
			this->B_Enable = (gcnew System::Windows::Forms::Button());
			this->C_Aut = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->T_port = (gcnew System::Windows::Forms::TextBox());
			this->B_RobPrev = (gcnew System::Windows::Forms::Button());
			this->B_RobNext = (gcnew System::Windows::Forms::Button());
			this->B_Run = (gcnew System::Windows::Forms::Button());
			this->B_Aut = (gcnew System::Windows::Forms::Button());
			this->P_INF = (gcnew System::Windows::Forms::Panel());
			this->L_INF = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->B_REM = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->L_Status = (gcnew System::Windows::Forms::Label());
			this->B_PROG_DOWNLOAD = (gcnew System::Windows::Forms::Button());
			this->B_PROG_SEARCH = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->T_PROG = (gcnew System::Windows::Forms::TextBox());
			this->B_COMReg = (gcnew System::Windows::Forms::Button());
			this->B_COM_SEARCH = (gcnew System::Windows::Forms::Button());
			this->P_SEARCH = (gcnew System::Windows::Forms::Panel());
			this->B_Search_Cancel = (gcnew System::Windows::Forms::Button());
			this->B_RESEARCH = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->P_INF->SuspendLayout();
			this->panel2->SuspendLayout();
			this->P_SEARCH->SuspendLayout();
			this->SuspendLayout();
			// 
			// L_RobNom
			// 
			this->L_RobNom->AutoSize = true;
			this->L_RobNom->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->L_RobNom->Location = System::Drawing::Point(146, 12);
			this->L_RobNom->Name = L"L_RobNom";
			this->L_RobNom->Size = System::Drawing::Size(16, 17);
			this->L_RobNom->TabIndex = 3;
			this->L_RobNom->Text = L"1";
			// 
			// C_Con
			// 
			this->C_Con->AutoSize = true;
			this->C_Con->BackColor = System::Drawing::SystemColors::Control;
			this->C_Con->Enabled = false;
			this->C_Con->Location = System::Drawing::Point(20, 151);
			this->C_Con->Name = L"C_Con";
			this->C_Con->Size = System::Drawing::Size(78, 17);
			this->C_Con->TabIndex = 8;
			this->C_Con->Text = L"Connected";
			this->C_Con->UseVisualStyleBackColor = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(32, 37);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(73, 13);
			this->label1->TabIndex = 12;
			this->label1->Text = L"MAC address:";
			// 
			// T_addr
			// 
			this->T_addr->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->T_addr->Enabled = false;
			this->T_addr->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->T_addr->Location = System::Drawing::Point(111, 35);
			this->T_addr->Name = L"T_addr";
			this->T_addr->Size = System::Drawing::Size(100, 20);
			this->T_addr->TabIndex = 14;
			this->T_addr->Text = L"00:00:00:00:00:00";
			// 
			// B_Con
			// 
			this->B_Con->AccessibleName = L"";
			this->B_Con->Location = System::Drawing::Point(111, 149);
			this->B_Con->Name = L"B_Con";
			this->B_Con->Size = System::Drawing::Size(74, 23);
			this->B_Con->TabIndex = 16;
			this->B_Con->Text = L"Connect";
			this->B_Con->UseVisualStyleBackColor = true;
			this->B_Con->Click += gcnew System::EventHandler(this, &Form1::B_Con_Click);
			// 
			// B_Dis
			// 
			this->B_Dis->Location = System::Drawing::Point(198, 149);
			this->B_Dis->Name = L"B_Dis";
			this->B_Dis->Size = System::Drawing::Size(75, 23);
			this->B_Dis->TabIndex = 17;
			this->B_Dis->Text = L"Disconnect";
			this->B_Dis->UseVisualStyleBackColor = true;
			this->B_Dis->Click += gcnew System::EventHandler(this, &Form1::B_Dis_Click);
			// 
			// B_Send
			// 
			this->B_Send->Location = System::Drawing::Point(19, 174);
			this->B_Send->Name = L"B_Send";
			this->B_Send->Size = System::Drawing::Size(86, 23);
			this->B_Send->TabIndex = 18;
			this->B_Send->Text = L"Send";
			this->B_Send->UseVisualStyleBackColor = true;
			this->B_Send->Click += gcnew System::EventHandler(this, &Form1::B_Send_Click);
			// 
			// T_Send
			// 
			this->T_Send->Location = System::Drawing::Point(111, 176);
			this->T_Send->Name = L"T_Send";
			this->T_Send->Size = System::Drawing::Size(182, 20);
			this->T_Send->TabIndex = 19;
			// 
			// B_Clean
			// 
			this->B_Clean->Location = System::Drawing::Point(284, 149);
			this->B_Clean->Name = L"B_Clean";
			this->B_Clean->Size = System::Drawing::Size(75, 23);
			this->B_Clean->TabIndex = 20;
			this->B_Clean->Text = L"Clean";
			this->B_Clean->UseVisualStyleBackColor = true;
			this->B_Clean->Click += gcnew System::EventHandler(this, &Form1::B_Clean_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(85, 12);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(63, 17);
			this->label3->TabIndex = 21;
			this->label3->Text = L"Device #";
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// B_SEARCH
			// 
			this->B_SEARCH->Location = System::Drawing::Point(217, 34);
			this->B_SEARCH->Name = L"B_SEARCH";
			this->B_SEARCH->Size = System::Drawing::Size(142, 22);
			this->B_SEARCH->TabIndex = 25;
			this->B_SEARCH->Text = L"Найти устройста...";
			this->B_SEARCH->UseVisualStyleBackColor = true;
			this->B_SEARCH->Click += gcnew System::EventHandler(this, &Form1::B_SEARCH_Click);
			// 
			// checkedListBox1
			// 
			this->checkedListBox1->FormattingEnabled = true;
			this->checkedListBox1->Location = System::Drawing::Point(4, 3);
			this->checkedListBox1->Name = L"checkedListBox1";
			this->checkedListBox1->Size = System::Drawing::Size(345, 124);
			this->checkedListBox1->TabIndex = 27;
			this->checkedListBox1->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &Form1::checkedListBox1_ItemCheck);
			// 
			// C_Dri
			// 
			this->C_Dri->AutoSize = true;
			this->C_Dri->BackColor = System::Drawing::SystemColors::Control;
			this->C_Dri->Enabled = false;
			this->C_Dri->Location = System::Drawing::Point(217, 65);
			this->C_Dri->Name = L"C_Dri";
			this->C_Dri->Size = System::Drawing::Size(73, 17);
			this->C_Dri->TabIndex = 7;
			this->C_Dri->Text = L"InstalDrier";
			this->C_Dri->UseVisualStyleBackColor = false;
			// 
			// panel1
			// 
			this->panel1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->pictureBox1);
			this->panel1->Location = System::Drawing::Point(249, 8);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(113, 25);
			this->panel1->TabIndex = 22;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 5);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(61, 13);
			this->label4->TabIndex = 2;
			this->label4->Text = L"Pipe Status";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(74, -5);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(32, 32);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(481, 4);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(40, 42);
			this->button1->TabIndex = 0;
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_1);
			// 
			// B_Disable
			// 
			this->B_Disable->Location = System::Drawing::Point(721, 4);
			this->B_Disable->Name = L"B_Disable";
			this->B_Disable->Size = System::Drawing::Size(142, 23);
			this->B_Disable->TabIndex = 11;
			this->B_Disable->Text = L"UnInstall";
			this->B_Disable->UseVisualStyleBackColor = true;
			this->B_Disable->Click += gcnew System::EventHandler(this, &Form1::B_Disable_Click);
			// 
			// B_Enable
			// 
			this->B_Enable->Location = System::Drawing::Point(615, 4);
			this->B_Enable->Name = L"B_Enable";
			this->B_Enable->Size = System::Drawing::Size(100, 23);
			this->B_Enable->TabIndex = 10;
			this->B_Enable->Text = L"Instal";
			this->B_Enable->UseVisualStyleBackColor = true;
			this->B_Enable->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// C_Aut
			// 
			this->C_Aut->AutoSize = true;
			this->C_Aut->BackColor = System::Drawing::SystemColors::Control;
			this->C_Aut->Enabled = false;
			this->C_Aut->Location = System::Drawing::Point(538, 14);
			this->C_Aut->Name = L"C_Aut";
			this->C_Aut->Size = System::Drawing::Size(92, 17);
			this->C_Aut->TabIndex = 6;
			this->C_Aut->Text = L"Authenticated";
			this->C_Aut->UseVisualStyleBackColor = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(50, 66);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(55, 13);
			this->label2->TabIndex = 13;
			this->label2->Text = L"COM port:";
			// 
			// T_port
			// 
			this->T_port->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->T_port->Enabled = false;
			this->T_port->Location = System::Drawing::Point(111, 64);
			this->T_port->MaxLength = 5;
			this->T_port->Name = L"T_port";
			this->T_port->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->T_port->Size = System::Drawing::Size(100, 20);
			this->T_port->TabIndex = 15;
			// 
			// B_RobPrev
			// 
			this->B_RobPrev->Location = System::Drawing::Point(6, 8);
			this->B_RobPrev->Name = L"B_RobPrev";
			this->B_RobPrev->Size = System::Drawing::Size(75, 25);
			this->B_RobPrev->TabIndex = 1;
			this->B_RobPrev->Text = L"Previev";
			this->B_RobPrev->UseVisualStyleBackColor = true;
			this->B_RobPrev->Click += gcnew System::EventHandler(this, &Form1::B_RobPrev_Click);
			// 
			// B_RobNext
			// 
			this->B_RobNext->Location = System::Drawing::Point(171, 8);
			this->B_RobNext->Name = L"B_RobNext";
			this->B_RobNext->Size = System::Drawing::Size(75, 25);
			this->B_RobNext->TabIndex = 4;
			this->B_RobNext->Text = L"Next";
			this->B_RobNext->UseVisualStyleBackColor = true;
			this->B_RobNext->Click += gcnew System::EventHandler(this, &Form1::B_RobNext_Click);
			// 
			// B_Run
			// 
			this->B_Run->Location = System::Drawing::Point(111, 119);
			this->B_Run->Name = L"B_Run";
			this->B_Run->Size = System::Drawing::Size(100, 22);
			this->B_Run->TabIndex = 0;
			this->B_Run->Text = L"Run Programm";
			this->B_Run->UseVisualStyleBackColor = true;
			this->B_Run->Click += gcnew System::EventHandler(this, &Form1::B_Run_Click);
			// 
			// B_Aut
			// 
			this->B_Aut->Location = System::Drawing::Point(235, 62);
			this->B_Aut->Name = L"B_Aut";
			this->B_Aut->Size = System::Drawing::Size(124, 23);
			this->B_Aut->TabIndex = 9;
			this->B_Aut->Text = L"Authenticate";
			this->B_Aut->UseVisualStyleBackColor = true;
			this->B_Aut->Click += gcnew System::EventHandler(this, &Form1::B_Aut_Click);
			// 
			// P_INF
			// 
			this->P_INF->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->P_INF->Controls->Add(this->L_INF);
			this->P_INF->Location = System::Drawing::Point(450, 35);
			this->P_INF->Name = L"P_INF";
			this->P_INF->Size = System::Drawing::Size(355, 60);
			this->P_INF->TabIndex = 23;
			this->P_INF->Visible = false;
			// 
			// L_INF
			// 
			this->L_INF->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom));
			this->L_INF->AutoSize = true;
			this->L_INF->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->L_INF->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->L_INF->Location = System::Drawing::Point(2, 18);
			this->L_INF->MinimumSize = System::Drawing::Size(350, 0);
			this->L_INF->Name = L"L_INF";
			this->L_INF->Size = System::Drawing::Size(350, 20);
			this->L_INF->TabIndex = 0;
			this->L_INF->Text = L"Message";
			this->L_INF->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->L_INF->UseMnemonic = false;
			// 
			// panel2
			// 
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel2->Controls->Add(this->B_REM);
			this->panel2->Controls->Add(this->label6);
			this->panel2->Controls->Add(this->L_Status);
			this->panel2->Controls->Add(this->B_PROG_DOWNLOAD);
			this->panel2->Controls->Add(this->B_PROG_SEARCH);
			this->panel2->Controls->Add(this->label5);
			this->panel2->Controls->Add(this->T_PROG);
			this->panel2->Controls->Add(this->B_SEARCH);
			this->panel2->Controls->Add(this->B_Aut);
			this->panel2->Controls->Add(this->B_Clean);
			this->panel2->Controls->Add(this->label2);
			this->panel2->Controls->Add(this->B_Run);
			this->panel2->Controls->Add(this->T_addr);
			this->panel2->Controls->Add(this->T_port);
			this->panel2->Controls->Add(this->T_Send);
			this->panel2->Controls->Add(this->B_Send);
			this->panel2->Controls->Add(this->C_Dri);
			this->panel2->Controls->Add(this->C_Con);
			this->panel2->Controls->Add(this->B_Dis);
			this->panel2->Controls->Add(this->B_Con);
			this->panel2->Controls->Add(this->label1);
			this->panel2->Location = System::Drawing::Point(0, 39);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(369, 201);
			this->panel2->TabIndex = 24;
			// 
			// B_REM
			// 
			this->B_REM->Location = System::Drawing::Point(299, 174);
			this->B_REM->Name = L"B_REM";
			this->B_REM->Size = System::Drawing::Size(27, 23);
			this->B_REM->TabIndex = 35;
			this->B_REM->Text = L"R";
			this->B_REM->UseVisualStyleBackColor = true;
			this->B_REM->Click += gcnew System::EventHandler(this, &Form1::B_REM_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(332, 181);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(13, 13);
			this->label6->TabIndex = 34;
			this->label6->Text = L"0";
			// 
			// L_Status
			// 
			this->L_Status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->L_Status->Location = System::Drawing::Point(32, 4);
			this->L_Status->Name = L"L_Status";
			this->L_Status->Size = System::Drawing::Size(294, 27);
			this->L_Status->TabIndex = 33;
			this->L_Status->Text = L"Status";
			this->L_Status->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// B_PROG_DOWNLOAD
			// 
			this->B_PROG_DOWNLOAD->Location = System::Drawing::Point(217, 119);
			this->B_PROG_DOWNLOAD->Name = L"B_PROG_DOWNLOAD";
			this->B_PROG_DOWNLOAD->Size = System::Drawing::Size(142, 22);
			this->B_PROG_DOWNLOAD->TabIndex = 32;
			this->B_PROG_DOWNLOAD->Text = L"Download";
			this->B_PROG_DOWNLOAD->UseVisualStyleBackColor = true;
			this->B_PROG_DOWNLOAD->Click += gcnew System::EventHandler(this, &Form1::B_PROG_DOWNLOAD_Click);
			// 
			// B_PROG_SEARCH
			// 
			this->B_PROG_SEARCH->Location = System::Drawing::Point(217, 89);
			this->B_PROG_SEARCH->Name = L"B_PROG_SEARCH";
			this->B_PROG_SEARCH->Size = System::Drawing::Size(142, 22);
			this->B_PROG_SEARCH->TabIndex = 30;
			this->B_PROG_SEARCH->Text = L"search";
			this->B_PROG_SEARCH->UseVisualStyleBackColor = true;
			this->B_PROG_SEARCH->Click += gcnew System::EventHandler(this, &Form1::B_PROG_SEARCH_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(23, 94);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(82, 13);
			this->label5->TabIndex = 31;
			this->label5->Text = L"NXT Programm:";
			// 
			// T_PROG
			// 
			this->T_PROG->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->T_PROG->Location = System::Drawing::Point(111, 91);
			this->T_PROG->Name = L"T_PROG";
			this->T_PROG->Size = System::Drawing::Size(100, 20);
			this->T_PROG->TabIndex = 30;
			// 
			// B_COMReg
			// 
			this->B_COMReg->Location = System::Drawing::Point(721, 29);
			this->B_COMReg->Name = L"B_COMReg";
			this->B_COMReg->Size = System::Drawing::Size(142, 22);
			this->B_COMReg->TabIndex = 30;
			this->B_COMReg->Text = L"Определить COM";
			this->B_COMReg->UseVisualStyleBackColor = true;
			this->B_COMReg->Click += gcnew System::EventHandler(this, &Form1::B_COMReg_Click);
			// 
			// B_COM_SEARCH
			// 
			this->B_COM_SEARCH->Location = System::Drawing::Point(379, 56);
			this->B_COM_SEARCH->Name = L"B_COM_SEARCH";
			this->B_COM_SEARCH->Size = System::Drawing::Size(89, 22);
			this->B_COM_SEARCH->TabIndex = 29;
			this->B_COM_SEARCH->Text = L"Найти COM порты...";
			this->B_COM_SEARCH->UseVisualStyleBackColor = true;
			this->B_COM_SEARCH->Click += gcnew System::EventHandler(this, &Form1::B_COM_SEARCH_Click);
			// 
			// P_SEARCH
			// 
			this->P_SEARCH->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->P_SEARCH->Controls->Add(this->B_Search_Cancel);
			this->P_SEARCH->Controls->Add(this->B_RESEARCH);
			this->P_SEARCH->Controls->Add(this->checkedListBox1);
			this->P_SEARCH->Enabled = false;
			this->P_SEARCH->Location = System::Drawing::Point(379, 77);
			this->P_SEARCH->Name = L"P_SEARCH";
			this->P_SEARCH->Size = System::Drawing::Size(356, 163);
			this->P_SEARCH->TabIndex = 28;
			this->P_SEARCH->Visible = false;
			// 
			// B_Search_Cancel
			// 
			this->B_Search_Cancel->Location = System::Drawing::Point(205, 133);
			this->B_Search_Cancel->Name = L"B_Search_Cancel";
			this->B_Search_Cancel->Size = System::Drawing::Size(118, 23);
			this->B_Search_Cancel->TabIndex = 29;
			this->B_Search_Cancel->Text = L"Cancel";
			this->B_Search_Cancel->UseVisualStyleBackColor = true;
			this->B_Search_Cancel->Click += gcnew System::EventHandler(this, &Form1::B_Search_Cancel_Click);
			// 
			// B_RESEARCH
			// 
			this->B_RESEARCH->Location = System::Drawing::Point(17, 132);
			this->B_RESEARCH->Name = L"B_RESEARCH";
			this->B_RESEARCH->Size = System::Drawing::Size(124, 23);
			this->B_RESEARCH->TabIndex = 28;
			this->B_RESEARCH->Text = L"ReSearch";
			this->B_RESEARCH->UseVisualStyleBackColor = true;
			this->B_RESEARCH->Click += gcnew System::EventHandler(this, &Form1::B_RESEARCH_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(379, 28);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 29;
			this->button2->Text = L"console";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog1_FileOk);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(379, 4);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 30;
			this->button3->Text = L"clear";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Menu;
			this->ClientSize = System::Drawing::Size(369, 240);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->B_COMReg);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->P_SEARCH);
			this->Controls->Add(this->P_INF);
			this->Controls->Add(this->B_COM_SEARCH);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->B_RobNext);
			this->Controls->Add(this->B_RobPrev);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->L_RobNom);
			this->Controls->Add(this->B_Disable);
			this->Controls->Add(this->B_Enable);
			this->Controls->Add(this->C_Aut);
			this->Name = L"Form1";
			this->Text = L"Connect Modul : BlueTouth - NXT ";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->P_INF->ResumeLayout(false);
			this->P_INF->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->P_SEARCH->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
#include "FormFunction.h"

private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 ShowConsole=!ShowConsole;
			 HWND console = GetConsoleWindow();
			
			if (!ShowConsole) 
			{
				ShowWindow(console, SW_HIDE);
			}
			else
			{
				ShowWindow(console, SW_SHOW);
			}
		 }
		 //pPipe->Send("123",100);
			/* 
			 char buf[100]="1111111111111111";
			 int sz=-1;
			while(!R[2].SgRecv(buf, &sz));
			//cout<<R[1].SgRecv(buf, &sz)<<" "<<sz<<endl;
			//pPipe->Send(buf,100);
	
			cout<<buf<<endl;*/
private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //pPipe->Send("OLOLO");
			 ClearLettons();
			/* int I=MessageBoxA(NULL,LPCSTR("Повторить попытку подключения?"),LPCSTR("Устройство не отвечает."),MB_YESNOCANCEL| MB_ICONINFORMATION);
			if (I==IDYES) ON("LOL YES");
			if (I==IDNO) ON("LOL NO");
			{
			}*/
			/*
			 BLUETOOTH_LOCAL_SERVICE_INFO I;
			 BluetoothSetLocalServiceInfo(0,R[1].guid,0,&I);
//			MessageBox(NULL,I.szDeviceString,"LOL",0);
			 cout<<I.Enabled<<endl;
			 char H[18];
			 ba2str(I.btAddr,H);
			 cout<<H<<endl;
			 cout<<(char)I.szDeviceString[0]<<(char)I.szDeviceString[1];
			 */// MessageBoxA(NULL,LPCSTR("Злая кнопка."),LPCSTR("Очень злая кнопка."),MB_DEFBUTTON1| MB_ICONINFORMATION);
			//OFF("Clean Connect...");
			//Sleep(1000);
			// COLOR++;
			//if (COLOR%2) pictureBox1->Image=image_But_OFF;
			//else pictureBox1->Image=image_But_ON;
			//ON();
			 //MESS("LOLO");
		 }

private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
		 {
		 }

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 ClearLettons();
		 }
private: System::Void B_REM_Click(System::Object^  sender, System::EventArgs^  e) 
{
	OFF("REMOTE CONTROL");		 
	char buf[32]="_____";
	buf[0]=0;
	buf[1]=0;
	buf[2]=0;
	buf[3]=0;
	buf[4]=0;

	int L,R,S,K,P;
	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		//
		L=0;R=0;S=0;K=0,P=0;
		if (GetAsyncKeyState('0'))
		{
			S=0;
		}
		if (GetAsyncKeyState('1'))
		{
			S=1;
		}
		if (GetAsyncKeyState('2'))
		{
			S=2;
		}
		if (GetAsyncKeyState('3'))
		{
			S=3;
		}
		if (GetAsyncKeyState('4'))
		{
			S=4;
		}
		if (GetAsyncKeyState('W'))
		{
			L+=50;
			R+=50;
		}
		if (GetAsyncKeyState('S'))
		{
			L-=50;
			R-=50;
		}
		if (GetAsyncKeyState('A'))
		{
			L-=25;
			R+=25;
		}
		if (GetAsyncKeyState('D'))
		{
			L+=25;
			R-=25;
		}
		if (GetAsyncKeyState(VK_SPACE)<0||GetAsyncKeyState('Q'))
			K=1;
		if (GetAsyncKeyState('E'))
			K=-1;
		if (GetAsyncKeyState('Z'))
			P=1;
		if (GetAsyncKeyState(VK_SHIFT)<0)
		{
			L=L*2;
			R=R*2;
			if (abs(L)>100) L=L/abs(L)*100;
			if (abs(R)>100) R=R/abs(R)*100;
		}
		cout<<L<<" "<<R<<" "<<K<<P<<S<<endl;
		buf[0]=L;
		buf[1]=R;
		buf[2]=K;
		buf[3]=P;
		buf[4]=S;
		SaveMes(buf,RobActNom);
		Sleep(100);
	}
	ON();
}
};
}

