#pragma once

#include "IInvFile.h"
namespace SearchEngine_Project {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	IInvFile InvFile;
	std::string post_filename ;
	std::string query_filename ;
	int stop = 0;
	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected: 


	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;

	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::TextBox^  textBox2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"SimSun", 13));
			this->button1->Location = System::Drawing::Point(538, 47);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(174, 52);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Search";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"SimSun", 13));
			this->button2->Location = System::Drawing::Point(538, 20);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(174, 52);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Generate";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// button3
			// 
			this->button3->Enabled = false;
			this->button3->Font = (gcnew System::Drawing::Font(L"SimSun", 13));
			this->button3->Location = System::Drawing::Point(538, 105);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(174, 52);
			this->button3->TabIndex = 2;
			this->button3->Text = L"Cancel";
			this->button3->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button5);
			this->groupBox1->Controls->Add(this->button4);
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Controls->Add(this->button2);
			this->groupBox1->Location = System::Drawing::Point(12, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(718, 138);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Get Inverted File";
			// 
			// button5
			// 
			this->button5->Enabled = false;
			this->button5->Font = (gcnew System::Drawing::Font(L"SimSun", 13));
			this->button5->Location = System::Drawing::Point(538, 78);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(174, 52);
			this->button5->TabIndex = 4;
			this->button5->Text = L"Cancel";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// button4
			// 
			this->button4->Font = (gcnew System::Drawing::Font(L"SimSun", 13));
			this->button4->Location = System::Drawing::Point(340, 20);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(174, 52);
			this->button4->TabIndex = 3;
			this->button4->Text = L"File";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"SimSun", 13));
			this->textBox1->Location = System::Drawing::Point(33, 35);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(301, 27);
			this->textBox1->TabIndex = 2;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->button6);
			this->groupBox2->Controls->Add(this->textBox2);
			this->groupBox2->Controls->Add(this->button1);
			this->groupBox2->Controls->Add(this->button3);
			this->groupBox2->Location = System::Drawing::Point(12, 156);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(718, 253);
			this->groupBox2->TabIndex = 4;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Search";
			this->groupBox2->Enter += gcnew System::EventHandler(this, &Form1::groupBox2_Enter);
			// 
			// button6
			// 
			this->button6->Font = (gcnew System::Drawing::Font(L"SimSun", 13));
			this->button6->Location = System::Drawing::Point(340, 47);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(174, 52);
			this->button6->TabIndex = 5;
			this->button6->Text = L"File";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// textBox2
			// 
			this->textBox2->Font = (gcnew System::Drawing::Font(L"SimSun", 13));
			this->textBox2->Location = System::Drawing::Point(33, 62);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(301, 27);
			this->textBox2->TabIndex = 4;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(742, 421);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void groupBox2_Enter(System::Object^  sender, System::EventArgs^  e) {
			 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				char tmp[10000];
				char str[1000];
				int docid;
				int loc;
				int cnt=0;
				if(post_filename == "")
				{
					System::Windows::Forms::MessageBox::Show("Please choose a file first! (By clicking 'file' button)");
					return;
				}
				const char * c = post_filename.c_str(); 
				FILE * fp = fopen(c , "rb");
				if (fp == NULL) {
					System::Windows::Forms::MessageBox::Show("Cannot open file \r\n");
					return;
				}
				button2->Enabled =false;
				button5->Enabled = true;
				// Initialize the Hash Table
				InvFile.MakeHashTable(13023973);

				while(fgets(tmp,10000,fp) != NULL) {
					// Get the stem, the document identifier and the location
					sscanf(tmp,"%s %d %d", &(str[0]), &docid, &loc);

					// Add posting into the Integrated Inverted index
					// See lecture notes on Implementation
					InvFile.Add(str, docid, 1);

					// Keep us informed about the progress
					cnt++;
					if ((cnt % 100000) == 0) printf("Added [%d]\r\n",cnt);
				}

				printf("Saving inverted file ...\r\n");
				InvFile.Save("InvFile.txt");

				InvFile.Clear();
				fclose(fp);
			}

		 void MarshalString ( String ^ s, string& os ) {
   using namespace Runtime::InteropServices;
   const char* chars = 
      (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}


private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			System::Windows::Forms::DialogResult result = openFileDialog1->ShowDialog();
			if (result == System::Windows::Forms::DialogResult::OK) // Test result.
			{
				MarshalString(openFileDialog1->FileName, post_filename);
				textBox1->Text = openFileDialog1->FileName;
			}

		}
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
			 stop = 1;
			 post_filename = "";
			 textBox1->Text = "";
			 button5->Enabled =false;
			 button2->Enabled = true;
		 }
private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
			System::Windows::Forms::DialogResult result = openFileDialog1->ShowDialog();
			if (result == System::Windows::Forms::DialogResult::OK) // Test result.
			{
				MarshalString(openFileDialog1->FileName, query_filename);
				textBox1->Text = openFileDialog1->FileName;
			}
		 }
};
}

