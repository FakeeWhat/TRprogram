#pragma once
#include "MyForm2.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>


namespace Project3 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Text::RegularExpressions;
	using namespace System::Collections::Generic;
	using namespace std;
	using namespace System::Text;
	using namespace System::Security::Cryptography;


	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}
		bool AuthenticateUser(String^ username, String^ password) {
			// Получаем хэш пароля пользователя
			SHA256Managed^ sha256 = gcnew SHA256Managed();
			cli::array<unsigned char>^ hashBytes = sha256->ComputeHash(Encoding::UTF8->GetBytes(password));

			// Преобразуем хэш в строку
			StringBuilder^ sb = gcnew StringBuilder();
			for each (unsigned char hashByte in hashBytes) {
				sb->Append(hashByte.ToString("x2"));
			}
			String^ hashedPassword = sb->ToString();

			// Считываем хэш пароля из файла
			ifstream file("passwords.txt");
			if (!file.is_open()) {
				MessageBox::Show("Ошибка: не удалось открыть файл с паролем", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
			string storedHash;
			file >> storedHash;
			file.close();

			// Сравниваем хэш-коды
			return hashedPassword->Equals(gcnew String(storedHash.c_str()));
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ but_exit;
	private: System::Windows::Forms::Button^ but_sign;

	private: System::Windows::Forms::Label^ label_main;

	private: System::Windows::Forms::Label^ label_user;

	private: System::Windows::Forms::Label^ label_pass;






	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: ZBobb::AlphaBlendTextBox^ text_user;
	private: ZBobb::AlphaBlendTextBox^ text_pass;
    private: System::Windows::Forms::PictureBox^ pictureBox1;
    private: System::Windows::Forms::Label^ label1;



	protected:

	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->but_exit = (gcnew System::Windows::Forms::Button());
			this->but_sign = (gcnew System::Windows::Forms::Button());
			this->label_main = (gcnew System::Windows::Forms::Label());
			this->label_user = (gcnew System::Windows::Forms::Label());
			this->label_pass = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->text_user = (gcnew ZBobb::AlphaBlendTextBox());
			this->text_pass = (gcnew ZBobb::AlphaBlendTextBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// but_exit
			// 
			this->but_exit->BackColor = System::Drawing::Color::Transparent;
			this->but_exit->FlatAppearance->BorderColor = System::Drawing::Color::White;
			this->but_exit->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->but_exit->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->but_exit->ForeColor = System::Drawing::Color::White;
			this->but_exit->Location = System::Drawing::Point(291, 463);
			this->but_exit->Name = L"but_exit";
			this->but_exit->Size = System::Drawing::Size(157, 57);
			this->but_exit->TabIndex = 0;
			this->but_exit->Text = L"Выход";
			this->but_exit->UseVisualStyleBackColor = false;
			this->but_exit->Click += gcnew System::EventHandler(this, &MyForm::but_exit_Click);
			// 
			// but_sign
			// 
			this->but_sign->BackColor = System::Drawing::Color::DimGray;
			this->but_sign->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->but_sign->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->but_sign->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->but_sign->ForeColor = System::Drawing::Color::White;
			this->but_sign->Location = System::Drawing::Point(118, 463);
			this->but_sign->Name = L"but_sign";
			this->but_sign->Size = System::Drawing::Size(159, 57);
			this->but_sign->TabIndex = 1;
			this->but_sign->Text = L"Вход";
			this->but_sign->UseVisualStyleBackColor = false;
			this->but_sign->Click += gcnew System::EventHandler(this, &MyForm::but_sign_Click);
			// 
			// label_main
			// 
			this->label_main->AutoSize = true;
			this->label_main->BackColor = System::Drawing::Color::Transparent;
			this->label_main->Cursor = System::Windows::Forms::Cursors::Default;
			this->label_main->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label_main->Font = (gcnew System::Drawing::Font(L"MS Reference Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label_main->ForeColor = System::Drawing::Color::White;
			this->label_main->Location = System::Drawing::Point(108, 142);
			this->label_main->Name = L"label_main";
			this->label_main->Size = System::Drawing::Size(149, 60);
			this->label_main->TabIndex = 2;
			this->label_main->Text = L"Вход";
			// 
			// label_user
			// 
			this->label_user->AutoSize = true;
			this->label_user->BackColor = System::Drawing::Color::Transparent;
			this->label_user->Font = (gcnew System::Drawing::Font(L"MS Reference Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label_user->ForeColor = System::Drawing::Color::White;
			this->label_user->Location = System::Drawing::Point(118, 227);
			this->label_user->Name = L"label_user";
			this->label_user->Size = System::Drawing::Size(205, 24);
			this->label_user->TabIndex = 3;
			this->label_user->Text = L"Имя пользователя";
			// 
			// label_pass
			// 
			this->label_pass->AutoSize = true;
			this->label_pass->BackColor = System::Drawing::Color::Transparent;
			this->label_pass->Font = (gcnew System::Drawing::Font(L"MS Reference Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label_pass->ForeColor = System::Drawing::Color::White;
			this->label_pass->Location = System::Drawing::Point(118, 314);
			this->label_pass->Name = L"label_pass";
			this->label_pass->Size = System::Drawing::Size(88, 24);
			this->label_pass->TabIndex = 4;
			this->label_pass->Text = L"Пароль";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::Transparent;
			this->label4->ForeColor = System::Drawing::SystemColors::Control;
			this->label4->Location = System::Drawing::Point(120, 288);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(325, 13);
			this->label4->TabIndex = 7;
			this->label4->Text = L"_____________________________________________________";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::Color::Transparent;
			this->label5->ForeColor = System::Drawing::SystemColors::Control;
			this->label5->Location = System::Drawing::Point(120, 370);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(331, 13);
			this->label5->TabIndex = 8;
			this->label5->Text = L"______________________________________________________";
			// 
			// text_user
			// 
			this->text_user->BackAlpha = 0;
			this->text_user->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->text_user->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->text_user->Font = (gcnew System::Drawing::Font(L"Segoe UI", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->text_user->ForeColor = System::Drawing::Color::White;
			this->text_user->Location = System::Drawing::Point(123, 269);
			this->text_user->Name = L"text_user";
			this->text_user->Size = System::Drawing::Size(325, 28);
			this->text_user->TabIndex = 9;
			// 
			// text_pass
			// 
			this->text_pass->BackAlpha = 0;
			this->text_pass->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->text_pass->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->text_pass->Font = (gcnew System::Drawing::Font(L"Segoe UI", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->text_pass->ForeColor = System::Drawing::Color::White;
			this->text_pass->Location = System::Drawing::Point(123, 351);
			this->text_pass->Name = L"text_pass";
			this->text_pass->PasswordChar = '*';
			this->text_pass->Size = System::Drawing::Size(325, 28);
			this->text_pass->TabIndex = 10;
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->ImageLocation = L"";
			this->pictureBox1->Location = System::Drawing::Point(0, 3);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(52, 33);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 11;
			this->pictureBox1->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(40, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(144, 32);
			this->label1->TabIndex = 12;
			this->label1->Text = L"TRprogram";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(1000, 650);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->text_pass);
			this->Controls->Add(this->text_user);
			this->Controls->Add(this->label_pass);
			this->Controls->Add(this->label_user);
			this->Controls->Add(this->label_main);
			this->Controls->Add(this->but_sign);
			this->Controls->Add(this->but_exit);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label5);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"TRprogram";
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseDown);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseMove);
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseUp);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void but_exit_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Close();
	}
private: System::Void but_sign_Click(System::Object^ sender, System::EventArgs^ e) {
	if (text_user->Text == "admin") {
		if (AuthenticateUser(text_user->Text, text_pass->Text)) {
			this->Hide();
			MyForm2^ form2 = gcnew MyForm2();
			form2->Show();
		}
		else {
			MessageBox::Show("Неправильный пароль", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
	else {
		MessageBox::Show("Неправильное имя пользователя", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}
	   bool dragging;
	   Point offset;
private: System::Void MyForm_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	dragging = true;
	offset.X = e->X;
	offset.Y = e->Y;
}
private: System::Void MyForm_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	if (dragging) {
		Point currentScreenPosition = PointToScreen(Point(e->X, e->Y));
		Location = Point(currentScreenPosition.X - offset.X, currentScreenPosition.Y - offset.Y);

	}
}
private: System::Void MyForm_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	dragging = false;
}
};
}
