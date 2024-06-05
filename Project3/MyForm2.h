#pragma once
#include <regex>

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

	/// <summary>
	/// Сводка для MyForm2
	/// </summary>
	public ref class MyForm2 : public System::Windows::Forms::Form
	{
	public:
		MyForm2(void)
		{
			InitializeComponent();
			panel_manage->Hide();
			panel_add->Hide();
			panel_change->Hide();
			panel_del->Hide();
			panel_look->Hide();
			panel_report->Hide();
			//
			//TODO: добавьте код конструктора
			//
		}

		value struct Record {
			String^ Model;
			String^ Size;
			String^ Name;
			String^ Number;
			String^ WeightWithCargo;
			String^ WeightWithoutCargo;
		};

		bool AreCarsExistForOwner(String^ ownerName) {
			String^ fileName = "records.bin"; // Имя файла с записями

			// Проверяем, существует ли файл
			if (!File::Exists(fileName)) {
				return false; // Если файла нет, автомобилей для владельца точно нет
			}

			// Открываем файл для чтения
			FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read);
			BinaryReader^ reader = gcnew BinaryReader(fs);

			try {
				// Проверяем каждую запись в файле
				while (reader->BaseStream->Position < reader->BaseStream->Length) {
					reader->ReadString(); // Пропускаем марку
					reader->ReadString(); // Пропускаем объем
					// Читаем ФИО владельца
					String^ savedOwnerName = reader->ReadString();

					// Если ФИО совпадает с искомым, значит, есть автомобиль для этого владельца
					if (savedOwnerName->Equals(ownerName)) {
						return true;
					}

					// Пропускаем остальные поля записи
					reader->ReadString(); // Пропускаем номер
					reader->ReadString(); // Пропускаем массу с грузом
					reader->ReadString(); // Пропускаем массу без груза
				}
			}
			catch (...) {
				// В случае ошибки возвращаем false
				return false;
			}
			finally {
				// Закрываем потоки
				reader->Close();
				fs->Close();
			}

			// Если запись с указанным владельцем не найдена, возвращаем false
			return false;
		}


		// Функция для проверки строки на соответствие регулярному выражению
		bool CheckRegex(String^ input, String^ pattern) {
			Regex^ regex = gcnew Regex(pattern);
			return regex->IsMatch(input);
		}

		
		// Функция для изменения записи по ФИО и номеру автомобиля
		void UpdateRecordByOwnerAndCarNumber(String^ fileName, String^ ownerName, String^ carNumber, String^ newBrand, String^ newsize, String^ newWeightWithCargo, String^ newWeightWithoutCargo, TextBox^ outputTextBox) {
			// Проверяем, существует ли файл
			if (!File::Exists(fileName)) {
				outputTextBox->AppendText("Файл не найден.\r\n");
				return;
			}

			// Открываем временный файл для записи
			String^ tempFileName = Path::GetTempFileName();
			FileStream^ tempFs = gcnew FileStream(tempFileName, FileMode::Create, FileAccess::Write);
			BinaryWriter^ writer = gcnew BinaryWriter(tempFs);

			// Открываем файл для чтения
			FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read);
			BinaryReader^ reader = gcnew BinaryReader(fs);

			try {
				// Копируем все записи из исходного файла во временный, обновляя при необходимости
				while (reader->BaseStream->Position < reader->BaseStream->Length) {
					String^ savedBrand = reader->ReadString(); // Марка
					String^ savedsize = reader->ReadString(); // Объем кузова
					String^ savedOwner = reader->ReadString(); // ФИО владельца
					String^ savedCarNumber = reader->ReadString(); // Номер автомобиля
					String^ savedWeightWithCargo = reader->ReadString(); // Масса с грузом
					String^ savedWeightWithoutCargo = reader->ReadString(); // Масса без груза

					if (savedOwner == ownerName && savedCarNumber == carNumber) {
						// Если ФИО и номер совпадают, записываем обновленные данные
						writer->Write(newBrand);
						writer->Write(newsize);
						writer->Write(ownerName);
						writer->Write(carNumber);
						writer->Write(newWeightWithCargo);
						writer->Write(newWeightWithoutCargo);
						outputTextBox->AppendText("Запись обновлена.\r\n");
					}
					else {
						// В противном случае копируем данные без изменений
						writer->Write(savedBrand);
						writer->Write(savedsize);
						writer->Write(savedOwner);
						writer->Write(savedCarNumber);
						writer->Write(savedWeightWithCargo);
						writer->Write(savedWeightWithoutCargo);
					}
				}

				// Закрываем файлы
				reader->Close();
				fs->Close();
				writer->Close();
				tempFs->Close();

				// Заменяем исходный файл временным
				File::Delete(fileName);
				File::Move(tempFileName, fileName);
			}
			catch (Exception^ e) {
				outputTextBox->AppendText("Ошибка при обновлении записи: " + e->Message + "\r\n");
				// Закрываем все файлы
				reader->Close();
				fs->Close();
				writer->Close();
				tempFs->Close();
				// Удаляем временный файл, если произошла ошибка
				File::Delete(tempFileName);
			}
		}

		// Функция для проверки существования записи с указанным ФИО и номером автомобиля
		bool IsRecordExistsByOwnerAndCarNumber(String^ fileName, String^ ownerName, String^ carNumber) {
			if (!File::Exists(fileName)) {
				return false;
			}

			FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read);
			BinaryReader^ reader = gcnew BinaryReader(fs);

			try {
				while (reader->BaseStream->Position < reader->BaseStream->Length) {
					reader->ReadString(); // Марка
					reader->ReadString(); // Объем
					// Читаем ФИО и номер автомобиля
					String^ savedOwnerName = reader->ReadString();
					String^ savedCarNumber = reader->ReadString();

					// Если ФИО и номер совпадают, возвращаем true
					if (savedOwnerName == ownerName && savedCarNumber == carNumber) {
						return true;
					}
					reader->ReadString(); // Масса с грузом
					reader->ReadString(); // Масса без груза
				}
			}
			catch (...) {
				// В случае ошибки возвращаем false
				return false;
			}
			finally {
				reader->Close();
				fs->Close();
			}

			return false; // Если запись не найдена, возвращаем false
		}
		
		// Функция для проверки корректности ввода данных
		String^ ValidateInputData(String^ brand, String^ size, String^ owner, String^ carNumber, String^ weightWithCargo, String^ weightWithoutCargo) {
			// Проверяем, что все поля не пустые
			if (String::IsNullOrWhiteSpace(brand) || String::IsNullOrWhiteSpace(size) || String::IsNullOrWhiteSpace(owner) ||
				String::IsNullOrWhiteSpace(carNumber) || String::IsNullOrWhiteSpace(weightWithCargo) || String::IsNullOrWhiteSpace(weightWithoutCargo)) {
				return "Не все поля заполнены.";
			}

			// Проверяем, что объем содержит только числа
			if (!Regex::Match(size, "^[0-9\\s.]+$")->Success) {
				return "Ошибка: Объем должен содержать только цифры и точки.";
			}

			// Проверяем, что ФИО содержит только буквы, пробелы и точки
			if (!Regex::Match(owner, "^[a-zA-Zа-яА-Я\\s.]+$")->Success) {
				return "Ошибка: ФИО должно содержать только буквы, пробелы и точки.";
			}

			// Проверяем, что номер автомобиля соответствует формату
			if (!IsValidCarNumber(carNumber)) {
				return "Ошибка: Номер автомобиля должен быть в формате Х000ХХ00 или Х000ХХ000.";
			}

			// Проверяем, что массы содержат только числа
			if (!Regex::Match(weightWithCargo, "^[0-9]+$")->Success || !Regex::Match(weightWithoutCargo, "^[0-9]+$")->Success) {
				return "Ошибка: Введите числовое значение для массы.";
			}

			return ""; // Если данные корректны, возвращаем пустую строку
		}

		// Функция для проверки корректности формата номера автомобиля
		bool IsValidCarNumber(String^ carNumber) {
			// Регулярное выражение для проверки формата номера автомобиля
			String^ pattern = "^[АВЕКМНОРСТУХ]\\d{3}[АВЕКМНОРСТУХ]{2}\\d{2,3}$";
			Regex^ regex = gcnew Regex(pattern);
			return regex->IsMatch(carNumber);
		}

		void AddRecord(String^ fileName, Record record) {
			// Открываем поток для записи в бинарном режиме
			FileStream^ fs = gcnew FileStream(fileName, FileMode::Append, FileAccess::Write);
			BinaryWriter^ writer = gcnew BinaryWriter(fs);

			try {
				// Записываем данные в файл
				writer->Write(record.Model);
				writer->Write(record.Size);
				writer->Write(record.Name);
				writer->Write(record.Number);
				writer->Write(record.WeightWithCargo);
				writer->Write(record.WeightWithoutCargo);

				// Добавляем информацию об успешной записи в поле вывода
				outputTextBox->AppendText("Запись успешно добавлена в файл:\r\n");
				outputTextBox->AppendText("Марка автомобиля: " + record.Model + "\r\n");
				outputTextBox->AppendText("Объем кузова (м^3): " + record.Size + "\r\n");
				outputTextBox->AppendText("ФИО владельца: " + record.Name + "\r\n");
				outputTextBox->AppendText("Номер автомобиля: " + record.Number + "\r\n");
				outputTextBox->AppendText("Масса с грузом (кг): " + record.WeightWithCargo + "\r\n");
				outputTextBox->AppendText("Масса без груза (кг): " + record.WeightWithoutCargo + "\r\n");
				outputTextBox->AppendText("-----------------------------------------\r\n");
			}
			catch (Exception^ e) {
				// Обрабатываем возможные ошибки
				outputTextBox->AppendText("Ошибка при записи в файл: " + e->Message + "\r\n");
			}
			finally {
				// Закрываем потоки
				writer->Close();
				fs->Close();
			}
		}

		// Функция для проверки, существует ли запись с указанным номером автомобиля
		bool IsCarNumberExists(String^ fileName, String^ carNumber) {
			// Проверяем, существует ли файл
			if (!File::Exists(fileName)) {
				return false;
			}

			// Открываем файл для чтения
			FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read);
			BinaryReader^ reader = gcnew BinaryReader(fs);

			try {
				// Проверяем каждую запись в файле
				while (reader->BaseStream->Position < reader->BaseStream->Length) {
					reader->ReadString(); // Пропускаем марку
					reader->ReadString(); // Пропускаем объем кузова
					reader->ReadString(); // Пропускаем ФИО владельца
					// Читаем номер автомобиля
					String^ savedCarNumber = reader->ReadString();
					// Если номер совпадает с искомым, возвращаем true
					if (savedCarNumber == carNumber) {
						return true;
					}
					reader->ReadString(); // Пропускаем массу с грузом
					reader->ReadString(); // Пропускаем массу без груза
				}
			}
			catch (Exception^) {
				// В случае ошибки просто возвращаем false
				return false;
			}
			finally {
				// Закрываем потоки
				reader->Close();
				fs->Close();
			}

			// Если номер не найден, возвращаем false
			return false;
		}


		// Функция для чтения всех записей из файла и вывода их в поле вывода
		void DisplayRecords(String^ fileName, TextBox^ outputTextBox) {
			// Очищаем поле вывода
			outputTextBox->Clear();

			// Проверяем, существует ли файл
			if (!File::Exists(fileName)) {
				outputTextBox->AppendText("Файл не найден.\r\n");
				return;
			}

			// Открываем файл для чтения
			FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read);
			BinaryReader^ reader = gcnew BinaryReader(fs);


			// Выводим предварительное сообщение
			outputTextBox->AppendText("Список всех записей с автомобилями:\r\n");
			outputTextBox->AppendText("-----------------------------------------\r\n");

			try {
				// Считываем и выводим записи из файла
				while (reader->BaseStream->Position < reader->BaseStream->Length) {
					outputTextBox->AppendText("Марка автомобиля: " + reader->ReadString() + "\r\n");
					outputTextBox->AppendText("Объем кузова (м^3): " + reader->ReadString() + "\r\n");
					outputTextBox->AppendText("ФИО владельца: " + reader->ReadString() + "\r\n");
					outputTextBox->AppendText("Номер автомобиля: " + reader->ReadString() + "\r\n");
					outputTextBox->AppendText("Масса с грузом (кг): " + reader->ReadString() + "\r\n");
					outputTextBox->AppendText("Масса без груза (кг): " + reader->ReadString() + "\r\n");
					outputTextBox->AppendText("-----------------------------------------\r\n");
				}
			}
			catch (Exception^ e) {
				outputTextBox->AppendText("Ошибка при чтении файла: " + e->Message + "\r\n");
			}
			finally {
				// Закрываем потоки
				reader->Close();
				fs->Close();
			}
		}

		// Функция для удаления записи с указанным номером автомобиля
		void RemoveRecordByCarNumber(String^ fileName, String^ carNumber, TextBox^ outputTextBox) {
			// Проверяем, существует ли файл
			if (!File::Exists(fileName)) {
				outputTextBox->AppendText("Файл не найден.\r\n");
				return;
			}

			// Проверяем, существует ли автомобиль с указанным номером в базе
			if (!IsCarNumberExists(fileName, carNumber)) {
				outputTextBox->AppendText("Автомобиль с номером " + carNumber + " не найден в базе.\r\n");
				return;
			}

			// Показываем диалоговое окно с подтверждением удаления
			String^ confirmMessage = "Вы уверены, что хотите удалить запись с номером автомобиля " + carNumber + "?";
			String^ confirmCaption = "Подтверждение удаления";
			MessageBoxButtons confirmButtons = MessageBoxButtons::YesNo;
			MessageBoxIcon confirmIcon = MessageBoxIcon::Warning;
			System::Windows::Forms::DialogResult confirmResult = MessageBox::Show(confirmMessage, confirmCaption, confirmButtons, confirmIcon);

			// Если пользователь подтвердил удаление, продолжаем
			if (confirmResult == System::Windows::Forms::DialogResult::Yes) {
				// Открываем временный файл для записи
				String^ tempFileName = Path::GetTempFileName();
				FileStream^ tempFs = gcnew FileStream(tempFileName, FileMode::Create, FileAccess::Write);
				BinaryWriter^ writer = gcnew BinaryWriter(tempFs);

				// Открываем файл для чтения
				FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read);
				BinaryReader^ reader = gcnew BinaryReader(fs);

				try {
					// Копируем все записи из исходного файла во временный, кроме той, которую нужно удалить
					while (reader->BaseStream->Position < reader->BaseStream->Length) {
						String^ savedData1 = reader->ReadString(); // Марка
						String^ savedData2 = reader->ReadString(); // Объем кузова
						String^ savedData3 = reader->ReadString(); // ФИО владельца
						String^ savedCarNumber = reader->ReadString(); // Номер автомобиля
						String^ savedData5 = reader->ReadString(); // Масса с грузом
						String^ savedData6 = reader->ReadString(); // Масса без груза

						if (savedCarNumber != carNumber) {
							writer->Write(savedData1);
							writer->Write(savedData2);
							writer->Write(savedData3);
							writer->Write(savedCarNumber);
							writer->Write(savedData5);
							writer->Write(savedData6);
						}
					}

					// Закрываем файлы
					reader->Close();
					fs->Close();
					writer->Close();
					tempFs->Close();

					// Заменяем исходный файл временным
					File::Delete(fileName);
					File::Move(tempFileName, fileName);

					outputTextBox->AppendText("Запись с номером автомобиля " + carNumber + " удалена.\r\n");
				}
				catch (Exception^ e) {
					outputTextBox->AppendText("Ошибка при удалении записи: " + e->Message + "\r\n");
					// Закрываем все файлы
					reader->Close();
					fs->Close();
					writer->Close();
					tempFs->Close();
					// Удаляем временный файл, если произошла ошибка
					File::Delete(tempFileName);
				}
			}
			else {
				outputTextBox->AppendText("Удаление записи отменено.\r\n");
			}
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm2()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ but_exit;
	private: System::Windows::Forms::TextBox^ outputTextBox;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::Panel^ panel2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::Button^ but_manage;







	private: System::Windows::Forms::Panel^ panel_manage;








private: System::Windows::Forms::Button^ button_change;

private: System::Windows::Forms::Button^ button_delete;
private: System::Windows::Forms::Button^ button_add;



	private: System::Windows::Forms::Panel^ panel_add;

	private: System::Windows::Forms::TextBox^ textBox2;
private: System::Windows::Forms::Button^ button_add1;


	private: System::Windows::Forms::Label^ label4;




	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TextBox^ textBox5;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::TextBox^ textBox4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TextBox^ textBox3;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::TextBox^ textBox7;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::TextBox^ textBox6;
	private: System::Windows::Forms::Panel^ panel_change;
private: System::Windows::Forms::Button^ button_change1;














	private: System::Windows::Forms::Label^ label16;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::TextBox^ textBox8;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::TextBox^ textBox9;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::TextBox^ textBox10;
	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::TextBox^ textBox11;
	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::TextBox^ textBox12;
	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::TextBox^ textBox13;
	private: System::Windows::Forms::Label^ label17;
	private: System::Windows::Forms::Panel^ panel3;
	private: System::Windows::Forms::Panel^ panel_del;



	private: System::Windows::Forms::Label^ label19;








	private: System::Windows::Forms::Label^ label24;
	private: System::Windows::Forms::TextBox^ textBox18;
private: System::Windows::Forms::Button^ button_delete1;






private: System::Windows::Forms::Panel^ panel_look;
private: System::Windows::Forms::Button^ button_display_models;



private: System::Windows::Forms::Button^ button_display_all;




private: System::Windows::Forms::Panel^ panel_report;
private: System::Windows::Forms::Button^ button_see_report;


private: System::Windows::Forms::Button^ button_create_report;



private: System::Windows::Forms::Label^ label18;
private: System::Windows::Forms::Label^ label20;
private: System::Windows::Forms::Panel^ panel4;
private: System::Windows::Forms::TextBox^ textBox14;
private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;
private: System::Windows::Forms::TextBox^ textBox1;
private: System::Windows::Forms::Label^ label21;
private: System::Windows::Forms::Button^ button3;

private: System::Windows::Forms::Button^ button4;
private: System::Windows::Forms::Panel^ SidePanel;
private: System::Windows::Forms::Button^ button1;





























































































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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm2::typeid));
			this->but_exit = (gcnew System::Windows::Forms::Button());
			this->outputTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SidePanel = (gcnew System::Windows::Forms::Panel());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->but_manage = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->panel_manage = (gcnew System::Windows::Forms::Panel());
			this->button_change = (gcnew System::Windows::Forms::Button());
			this->button_delete = (gcnew System::Windows::Forms::Button());
			this->button_add = (gcnew System::Windows::Forms::Button());
			this->panel_del = (gcnew System::Windows::Forms::Panel());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->textBox18 = (gcnew System::Windows::Forms::TextBox());
			this->button_delete1 = (gcnew System::Windows::Forms::Button());
			this->panel_change = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->textBox10 = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->textBox11 = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->textBox12 = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->textBox13 = (gcnew System::Windows::Forms::TextBox());
			this->button_change1 = (gcnew System::Windows::Forms::Button());
			this->panel_add = (gcnew System::Windows::Forms::Panel());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button_add1 = (gcnew System::Windows::Forms::Button());
			this->panel_look = (gcnew System::Windows::Forms::Panel());
			this->button_display_models = (gcnew System::Windows::Forms::Button());
			this->button_display_all = (gcnew System::Windows::Forms::Button());
			this->panel_report = (gcnew System::Windows::Forms::Panel());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->textBox14 = (gcnew System::Windows::Forms::TextBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->button_see_report = (gcnew System::Windows::Forms::Button());
			this->button_create_report = (gcnew System::Windows::Forms::Button());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label21 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->panel_manage->SuspendLayout();
			this->panel_del->SuspendLayout();
			this->panel_change->SuspendLayout();
			this->panel_add->SuspendLayout();
			this->panel_look->SuspendLayout();
			this->panel_report->SuspendLayout();
			this->SuspendLayout();
			// 
			// but_exit
			// 
			this->but_exit->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->but_exit->AutoSize = true;
			this->but_exit->BackColor = System::Drawing::Color::Black;
			this->but_exit->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->but_exit->FlatAppearance->BorderSize = 0;
			this->but_exit->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->but_exit->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->but_exit->ForeColor = System::Drawing::Color::White;
			this->but_exit->Location = System::Drawing::Point(962, 0);
			this->but_exit->Name = L"but_exit";
			this->but_exit->Size = System::Drawing::Size(38, 36);
			this->but_exit->TabIndex = 0;
			this->but_exit->Text = L"X";
			this->but_exit->UseVisualStyleBackColor = false;
			this->but_exit->Click += gcnew System::EventHandler(this, &MyForm2::button1_Click);
			// 
			// outputTextBox
			// 
			this->outputTextBox->BackColor = System::Drawing::Color::DimGray;
			this->outputTextBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->outputTextBox->ForeColor = System::Drawing::Color::White;
			this->outputTextBox->Location = System::Drawing::Point(265, 428);
			this->outputTextBox->Multiline = true;
			this->outputTextBox->Name = L"outputTextBox";
			this->outputTextBox->ReadOnly = true;
			this->outputTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->outputTextBox->Size = System::Drawing::Size(723, 210);
			this->outputTextBox->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Black;
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(40, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(144, 32);
			this->label1->TabIndex = 2;
			this->label1->Text = L"TRprogram";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label2->Location = System::Drawing::Point(261, 405);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(119, 21);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Поле вывода:";
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Black;
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->ImageLocation = L"";
			this->pictureBox1->Location = System::Drawing::Point(0, 3);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(52, 33);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 4;
			this->pictureBox1->TabStop = false;
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::Black;
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->pictureBox1);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Top;
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(1000, 36);
			this->panel1->TabIndex = 5;
			this->panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm2::panel1_MouseDown);
			this->panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm2::panel1_MouseMove);
			this->panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm2::panel1_MouseUp);
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::DimGray;
			this->panel2->Controls->Add(this->button1);
			this->panel2->Controls->Add(this->SidePanel);
			this->panel2->Controls->Add(this->button3);
			this->panel2->Controls->Add(this->button4);
			this->panel2->Controls->Add(this->but_manage);
			this->panel2->Controls->Add(this->label3);
			this->panel2->Controls->Add(this->pictureBox2);
			this->panel2->Dock = System::Windows::Forms::DockStyle::Left;
			this->panel2->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->panel2->Location = System::Drawing::Point(0, 36);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(250, 614);
			this->panel2->TabIndex = 0;
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::DimGray;
			this->button1->FlatAppearance->BorderSize = 0;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button1.Image")));
			this->button1->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->button1->Location = System::Drawing::Point(8, 115);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(242, 50);
			this->button1->TabIndex = 12;
			this->button1->Text = L"             Главная";
			this->button1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm2::button1_Click_3);
			// 
			// SidePanel
			// 
			this->SidePanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(153)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->SidePanel->Location = System::Drawing::Point(0, 115);
			this->SidePanel->Name = L"SidePanel";
			this->SidePanel->Size = System::Drawing::Size(8, 50);
			this->SidePanel->TabIndex = 11;
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::DimGray;
			this->button3->FlatAppearance->BorderSize = 0;
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button3->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button3.Image")));
			this->button3->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->button3->Location = System::Drawing::Point(8, 281);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(242, 50);
			this->button3->TabIndex = 10;
			this->button3->Text = L"          Отчёт по владельцам";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm2::button3_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::Color::DimGray;
			this->button4->FlatAppearance->BorderSize = 0;
			this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button4->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button4->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button4.Image")));
			this->button4->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->button4->Location = System::Drawing::Point(8, 225);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(242, 50);
			this->button4->TabIndex = 9;
			this->button4->Text = L"    Просмотр записей";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm2::button4_Click);
			// 
			// but_manage
			// 
			this->but_manage->BackColor = System::Drawing::Color::DimGray;
			this->but_manage->FlatAppearance->BorderSize = 0;
			this->but_manage->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->but_manage->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->but_manage->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"but_manage.Image")));
			this->but_manage->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->but_manage->Location = System::Drawing::Point(8, 170);
			this->but_manage->Name = L"but_manage";
			this->but_manage->Size = System::Drawing::Size(242, 50);
			this->but_manage->TabIndex = 2;
			this->but_manage->Text = L"           Управление записями";
			this->but_manage->UseVisualStyleBackColor = false;
			this->but_manage->Click += gcnew System::EventHandler(this, &MyForm2::button1_Click_1);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label3->ForeColor = System::Drawing::Color::Black;
			this->label3->Location = System::Drawing::Point(76, 36);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(81, 32);
			this->label3->TabIndex = 1;
			this->label3->Text = L"admin";
			// 
			// pictureBox2
			// 
			this->pictureBox2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(12, 20);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(58, 63);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox2->TabIndex = 0;
			this->pictureBox2->TabStop = false;
			// 
			// panel_manage
			// 
			this->panel_manage->Controls->Add(this->button_change);
			this->panel_manage->Controls->Add(this->button_delete);
			this->panel_manage->Controls->Add(this->button_add);
			this->panel_manage->Controls->Add(this->panel_del);
			this->panel_manage->Controls->Add(this->panel_change);
			this->panel_manage->Controls->Add(this->panel_add);
			this->panel_manage->Location = System::Drawing::Point(250, 36);
			this->panel_manage->Name = L"panel_manage";
			this->panel_manage->Size = System::Drawing::Size(750, 367);
			this->panel_manage->TabIndex = 6;
			this->panel_manage->Visible = false;
			// 
			// button_change
			// 
			this->button_change->AutoSize = true;
			this->button_change->FlatAppearance->BorderSize = 0;
			this->button_change->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_change->Font = (gcnew System::Drawing::Font(L"Segoe UI", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_change->ForeColor = System::Drawing::Color::Black;
			this->button_change->Location = System::Drawing::Point(15, 155);
			this->button_change->Name = L"button_change";
			this->button_change->Size = System::Drawing::Size(175, 45);
			this->button_change->TabIndex = 8;
			this->button_change->Text = L"Изменение";
			this->button_change->UseVisualStyleBackColor = true;
			this->button_change->Click += gcnew System::EventHandler(this, &MyForm2::button8_Click);
			// 
			// button_delete
			// 
			this->button_delete->AutoSize = true;
			this->button_delete->FlatAppearance->BorderSize = 0;
			this->button_delete->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_delete->Font = (gcnew System::Drawing::Font(L"Segoe UI", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_delete->ForeColor = System::Drawing::Color::Black;
			this->button_delete->Location = System::Drawing::Point(15, 245);
			this->button_delete->Name = L"button_delete";
			this->button_delete->Size = System::Drawing::Size(175, 45);
			this->button_delete->TabIndex = 7;
			this->button_delete->Text = L"Удаление";
			this->button_delete->UseVisualStyleBackColor = true;
			this->button_delete->Click += gcnew System::EventHandler(this, &MyForm2::button7_Click);
			// 
			// button_add
			// 
			this->button_add->AutoSize = true;
			this->button_add->FlatAppearance->BorderSize = 0;
			this->button_add->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_add->Font = (gcnew System::Drawing::Font(L"Segoe UI", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_add->ForeColor = System::Drawing::Color::Black;
			this->button_add->Location = System::Drawing::Point(15, 65);
			this->button_add->Name = L"button_add";
			this->button_add->Size = System::Drawing::Size(175, 45);
			this->button_add->TabIndex = 6;
			this->button_add->Text = L"Добавление";
			this->button_add->UseVisualStyleBackColor = true;
			this->button_add->Click += gcnew System::EventHandler(this, &MyForm2::button6_Click);
			// 
			// panel_del
			// 
			this->panel_del->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->panel_del->BackColor = System::Drawing::Color::LightGray;
			this->panel_del->Controls->Add(this->label19);
			this->panel_del->Controls->Add(this->label24);
			this->panel_del->Controls->Add(this->textBox18);
			this->panel_del->Controls->Add(this->button_delete1);
			this->panel_del->Location = System::Drawing::Point(223, 23);
			this->panel_del->Name = L"panel_del";
			this->panel_del->Size = System::Drawing::Size(514, 326);
			this->panel_del->TabIndex = 16;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label19->ForeColor = System::Drawing::Color::Black;
			this->label19->Location = System::Drawing::Point(25, 10);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(164, 20);
			this->label19->TabIndex = 13;
			this->label19->Text = L"Данные для удаления:";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label24->ForeColor = System::Drawing::Color::Black;
			this->label24->Location = System::Drawing::Point(25, 40);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(466, 20);
			this->label24->TabIndex = 4;
			this->label24->Text = L"Номер автомобиля, который хотите удалить (шаблон: Х000ХХ00):";
			// 
			// textBox18
			// 
			this->textBox18->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox18->Location = System::Drawing::Point(25, 64);
			this->textBox18->Name = L"textBox18";
			this->textBox18->Size = System::Drawing::Size(158, 27);
			this->textBox18->TabIndex = 3;
			// 
			// button_delete1
			// 
			this->button_delete1->BackColor = System::Drawing::Color::White;
			this->button_delete1->FlatAppearance->BorderSize = 0;
			this->button_delete1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_delete1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_delete1->ForeColor = System::Drawing::Color::Black;
			this->button_delete1->Location = System::Drawing::Point(24, 264);
			this->button_delete1->Name = L"button_delete1";
			this->button_delete1->Size = System::Drawing::Size(116, 46);
			this->button_delete1->TabIndex = 0;
			this->button_delete1->Text = L"Удалить";
			this->button_delete1->UseVisualStyleBackColor = false;
			this->button_delete1->Click += gcnew System::EventHandler(this, &MyForm2::button11_Click);
			// 
			// panel_change
			// 
			this->panel_change->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->panel_change->BackColor = System::Drawing::Color::LightGray;
			this->panel_change->Controls->Add(this->panel3);
			this->panel_change->Controls->Add(this->label17);
			this->panel_change->Controls->Add(this->label16);
			this->panel_change->Controls->Add(this->label10);
			this->panel_change->Controls->Add(this->textBox8);
			this->panel_change->Controls->Add(this->label11);
			this->panel_change->Controls->Add(this->textBox9);
			this->panel_change->Controls->Add(this->label12);
			this->panel_change->Controls->Add(this->textBox10);
			this->panel_change->Controls->Add(this->label13);
			this->panel_change->Controls->Add(this->textBox11);
			this->panel_change->Controls->Add(this->label14);
			this->panel_change->Controls->Add(this->textBox12);
			this->panel_change->Controls->Add(this->label15);
			this->panel_change->Controls->Add(this->textBox13);
			this->panel_change->Controls->Add(this->button_change1);
			this->panel_change->Location = System::Drawing::Point(223, 23);
			this->panel_change->Name = L"panel_change";
			this->panel_change->Size = System::Drawing::Size(514, 326);
			this->panel_change->TabIndex = 10;
			// 
			// panel3
			// 
			this->panel3->BackColor = System::Drawing::Color::Black;
			this->panel3->Location = System::Drawing::Point(252, 19);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(1, 281);
			this->panel3->TabIndex = 15;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label17->ForeColor = System::Drawing::Color::Black;
			this->label17->Location = System::Drawing::Point(271, 10);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(116, 20);
			this->label17->TabIndex = 14;
			this->label17->Text = L"Новые данные:";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label16->ForeColor = System::Drawing::Color::Black;
			this->label16->Location = System::Drawing::Point(25, 10);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(177, 20);
			this->label16->TabIndex = 13;
			this->label16->Text = L"Данные для изменения:";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label10->ForeColor = System::Drawing::Color::Black;
			this->label10->Location = System::Drawing::Point(271, 206);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(240, 20);
			this->label10->TabIndex = 12;
			this->label10->Text = L"Фактическая масса без груза (кг):";
			// 
			// textBox8
			// 
			this->textBox8->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox8->Location = System::Drawing::Point(271, 230);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(131, 27);
			this->textBox8->TabIndex = 11;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label11->ForeColor = System::Drawing::Color::Black;
			this->label11->Location = System::Drawing::Point(271, 152);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(235, 20);
			this->label11->TabIndex = 10;
			this->label11->Text = L"Фактическая масса с грузом (кг):";
			// 
			// textBox9
			// 
			this->textBox9->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox9->Location = System::Drawing::Point(271, 176);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(131, 27);
			this->textBox9->TabIndex = 9;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label12->ForeColor = System::Drawing::Color::Black;
			this->label12->Location = System::Drawing::Point(271, 94);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(152, 20);
			this->label12->TabIndex = 8;
			this->label12->Text = L"Обьём кузова (м^3):";
			// 
			// textBox10
			// 
			this->textBox10->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox10->Location = System::Drawing::Point(271, 118);
			this->textBox10->Name = L"textBox10";
			this->textBox10->Size = System::Drawing::Size(158, 27);
			this->textBox10->TabIndex = 7;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label13->ForeColor = System::Drawing::Color::Black;
			this->label13->Location = System::Drawing::Point(24, 40);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(122, 20);
			this->label13->TabIndex = 6;
			this->label13->Text = L"ФИО владельца:";
			// 
			// textBox11
			// 
			this->textBox11->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox11->Location = System::Drawing::Point(24, 64);
			this->textBox11->Name = L"textBox11";
			this->textBox11->Size = System::Drawing::Size(214, 27);
			this->textBox11->TabIndex = 5;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label14->ForeColor = System::Drawing::Color::Black;
			this->label14->Location = System::Drawing::Point(24, 94);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(149, 20);
			this->label14->TabIndex = 4;
			this->label14->Text = L"Номер автомобиля:";
			// 
			// textBox12
			// 
			this->textBox12->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox12->Location = System::Drawing::Point(24, 118);
			this->textBox12->Name = L"textBox12";
			this->textBox12->Size = System::Drawing::Size(158, 27);
			this->textBox12->TabIndex = 3;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label15->ForeColor = System::Drawing::Color::Black;
			this->label15->Location = System::Drawing::Point(271, 40);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(146, 20);
			this->label15->TabIndex = 2;
			this->label15->Text = L"Марка автомобиля:";
			// 
			// textBox13
			// 
			this->textBox13->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox13->Location = System::Drawing::Point(271, 64);
			this->textBox13->Name = L"textBox13";
			this->textBox13->Size = System::Drawing::Size(221, 27);
			this->textBox13->TabIndex = 1;
			// 
			// button_change1
			// 
			this->button_change1->BackColor = System::Drawing::Color::White;
			this->button_change1->FlatAppearance->BorderSize = 0;
			this->button_change1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_change1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_change1->ForeColor = System::Drawing::Color::Black;
			this->button_change1->Location = System::Drawing::Point(24, 264);
			this->button_change1->Name = L"button_change1";
			this->button_change1->Size = System::Drawing::Size(116, 46);
			this->button_change1->TabIndex = 0;
			this->button_change1->Text = L"Изменить";
			this->button_change1->UseVisualStyleBackColor = false;
			this->button_change1->Click += gcnew System::EventHandler(this, &MyForm2::button10_Click);
			// 
			// panel_add
			// 
			this->panel_add->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->panel_add->BackColor = System::Drawing::Color::LightGray;
			this->panel_add->Controls->Add(this->label9);
			this->panel_add->Controls->Add(this->textBox7);
			this->panel_add->Controls->Add(this->label8);
			this->panel_add->Controls->Add(this->textBox6);
			this->panel_add->Controls->Add(this->label7);
			this->panel_add->Controls->Add(this->textBox5);
			this->panel_add->Controls->Add(this->label6);
			this->panel_add->Controls->Add(this->textBox4);
			this->panel_add->Controls->Add(this->label5);
			this->panel_add->Controls->Add(this->textBox3);
			this->panel_add->Controls->Add(this->label4);
			this->panel_add->Controls->Add(this->textBox2);
			this->panel_add->Controls->Add(this->button_add1);
			this->panel_add->Location = System::Drawing::Point(223, 23);
			this->panel_add->Name = L"panel_add";
			this->panel_add->Size = System::Drawing::Size(514, 326);
			this->panel_add->TabIndex = 9;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label9->ForeColor = System::Drawing::Color::Black;
			this->label9->Location = System::Drawing::Point(234, 90);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(248, 21);
			this->label9->TabIndex = 12;
			this->label9->Text = L"Фактическая масса без груза (кг):";
			// 
			// textBox7
			// 
			this->textBox7->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox7->Location = System::Drawing::Point(234, 114);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(131, 29);
			this->textBox7->TabIndex = 11;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label8->ForeColor = System::Drawing::Color::Black;
			this->label8->Location = System::Drawing::Point(234, 19);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(243, 21);
			this->label8->TabIndex = 10;
			this->label8->Text = L"Фактическая масса с грузом (кг):";
			// 
			// textBox6
			// 
			this->textBox6->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox6->Location = System::Drawing::Point(234, 43);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(131, 29);
			this->textBox6->TabIndex = 9;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label7->ForeColor = System::Drawing::Color::Black;
			this->label7->Location = System::Drawing::Point(28, 92);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(158, 21);
			this->label7->TabIndex = 8;
			this->label7->Text = L"Обьём кузова (м^3):";
			// 
			// textBox5
			// 
			this->textBox5->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox5->Location = System::Drawing::Point(28, 116);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(158, 29);
			this->textBox5->TabIndex = 7;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label6->ForeColor = System::Drawing::Color::Black;
			this->label6->Location = System::Drawing::Point(28, 162);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(466, 21);
			this->label6->TabIndex = 6;
			this->label6->Text = L"ФИО владельца автомобиля (шаблон: Иванов Иван Иванович):";
			// 
			// textBox4
			// 
			this->textBox4->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox4->Location = System::Drawing::Point(28, 186);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(158, 29);
			this->textBox4->TabIndex = 5;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label5->ForeColor = System::Drawing::Color::Black;
			this->label5->Location = System::Drawing::Point(28, 233);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(300, 21);
			this->label5->TabIndex = 4;
			this->label5->Text = L"Номер автомобиля (шаблон: Х000ХХ00):";
			// 
			// textBox3
			// 
			this->textBox3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox3->Location = System::Drawing::Point(28, 257);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(158, 29);
			this->textBox3->TabIndex = 3;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label4->ForeColor = System::Drawing::Color::Black;
			this->label4->Location = System::Drawing::Point(28, 19);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(150, 21);
			this->label4->TabIndex = 2;
			this->label4->Text = L"Марка автомобиля:";
			// 
			// textBox2
			// 
			this->textBox2->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox2->Location = System::Drawing::Point(28, 43);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(162, 29);
			this->textBox2->TabIndex = 1;
			// 
			// button_add1
			// 
			this->button_add1->BackColor = System::Drawing::Color::White;
			this->button_add1->FlatAppearance->BorderSize = 0;
			this->button_add1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_add1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_add1->ForeColor = System::Drawing::Color::Black;
			this->button_add1->Location = System::Drawing::Point(376, 260);
			this->button_add1->Name = L"button_add1";
			this->button_add1->Size = System::Drawing::Size(116, 46);
			this->button_add1->TabIndex = 0;
			this->button_add1->Text = L"Добавить";
			this->button_add1->UseVisualStyleBackColor = false;
			this->button_add1->Click += gcnew System::EventHandler(this, &MyForm2::button_add_Click);
			// 
			// panel_look
			// 
			this->panel_look->Controls->Add(this->button_display_models);
			this->panel_look->Controls->Add(this->button_display_all);
			this->panel_look->Location = System::Drawing::Point(250, 36);
			this->panel_look->Name = L"panel_look";
			this->panel_look->Size = System::Drawing::Size(750, 367);
			this->panel_look->TabIndex = 17;
			this->panel_look->Visible = false;
			// 
			// button_display_models
			// 
			this->button_display_models->AutoSize = true;
			this->button_display_models->FlatAppearance->BorderSize = 0;
			this->button_display_models->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_display_models->Font = (gcnew System::Drawing::Font(L"Segoe UI", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_display_models->ForeColor = System::Drawing::Color::Black;
			this->button_display_models->Location = System::Drawing::Point(15, 155);
			this->button_display_models->Name = L"button_display_models";
			this->button_display_models->Size = System::Drawing::Size(249, 45);
			this->button_display_models->TabIndex = 8;
			this->button_display_models->Text = L"Марки и их количество";
			this->button_display_models->UseVisualStyleBackColor = true;
			this->button_display_models->Click += gcnew System::EventHandler(this, &MyForm2::button12_Click);
			// 
			// button_display_all
			// 
			this->button_display_all->AutoSize = true;
			this->button_display_all->FlatAppearance->BorderSize = 0;
			this->button_display_all->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_display_all->Font = (gcnew System::Drawing::Font(L"Segoe UI", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_display_all->ForeColor = System::Drawing::Color::Black;
			this->button_display_all->Location = System::Drawing::Point(15, 65);
			this->button_display_all->Name = L"button_display_all";
			this->button_display_all->Size = System::Drawing::Size(175, 45);
			this->button_display_all->TabIndex = 6;
			this->button_display_all->Text = L"Все записи";
			this->button_display_all->UseVisualStyleBackColor = true;
			this->button_display_all->Click += gcnew System::EventHandler(this, &MyForm2::button14_Click);
			// 
			// panel_report
			// 
			this->panel_report->Controls->Add(this->panel4);
			this->panel_report->Controls->Add(this->textBox14);
			this->panel_report->Controls->Add(this->label20);
			this->panel_report->Controls->Add(this->label18);
			this->panel_report->Controls->Add(this->button_see_report);
			this->panel_report->Controls->Add(this->button_create_report);
			this->panel_report->Location = System::Drawing::Point(250, 36);
			this->panel_report->Name = L"panel_report";
			this->panel_report->Size = System::Drawing::Size(750, 367);
			this->panel_report->TabIndex = 18;
			this->panel_report->Visible = false;
			// 
			// panel4
			// 
			this->panel4->BackColor = System::Drawing::Color::Black;
			this->panel4->Location = System::Drawing::Point(17, 118);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(229, 2);
			this->panel4->TabIndex = 12;
			// 
			// textBox14
			// 
			this->textBox14->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox14->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->textBox14->Location = System::Drawing::Point(17, 98);
			this->textBox14->Name = L"textBox14";
			this->textBox14->Size = System::Drawing::Size(229, 22);
			this->textBox14->TabIndex = 11;
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label20->ForeColor = System::Drawing::Color::Black;
			this->label20->Location = System::Drawing::Point(194, 277);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(325, 21);
			this->label20->TabIndex = 10;
			this->label20->Text = L"Выдать на экран содержимое файла-отчёта";
			// 
			// label18
			// 
			this->label18->Font = (gcnew System::Drawing::Font(L"Segoe UI Semibold", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label18->ForeColor = System::Drawing::Color::Black;
			this->label18->Location = System::Drawing::Point(11, 47);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(337, 48);
			this->label18->TabIndex = 9;
			this->label18->Text = L"ФИО владельца для формирования файла-отчёта:";
			// 
			// button_see_report
			// 
			this->button_see_report->AutoSize = true;
			this->button_see_report->FlatAppearance->BorderSize = 0;
			this->button_see_report->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_see_report->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_see_report->ForeColor = System::Drawing::Color::Black;
			this->button_see_report->Location = System::Drawing::Point(15, 269);
			this->button_see_report->Name = L"button_see_report";
			this->button_see_report->Size = System::Drawing::Size(173, 35);
			this->button_see_report->TabIndex = 8;
			this->button_see_report->Text = L"Просмотр отчёта";
			this->button_see_report->UseVisualStyleBackColor = true;
			this->button_see_report->Click += gcnew System::EventHandler(this, &MyForm2::button1_Click_2);
			// 
			// button_create_report
			// 
			this->button_create_report->AutoSize = true;
			this->button_create_report->FlatAppearance->BorderSize = 0;
			this->button_create_report->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button_create_report->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button_create_report->ForeColor = System::Drawing::Color::Black;
			this->button_create_report->Location = System::Drawing::Point(15, 131);
			this->button_create_report->Name = L"button_create_report";
			this->button_create_report->Size = System::Drawing::Size(196, 35);
			this->button_create_report->TabIndex = 6;
			this->button_create_report->Text = L"Создать файл-отчёт";
			this->button_create_report->UseVisualStyleBackColor = true;
			this->button_create_report->Click += gcnew System::EventHandler(this, &MyForm2::button13_Click);
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::Color::White;
			this->textBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox1->Enabled = false;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox1->ForeColor = System::Drawing::Color::Black;
			this->textBox1->Location = System::Drawing::Point(284, 141);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(543, 234);
			this->textBox1->TabIndex = 19;
			this->textBox1->Text = resources->GetString(L"textBox1.Text");
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->BackColor = System::Drawing::Color::White;
			this->label21->Font = (gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label21->ForeColor = System::Drawing::Color::Black;
			this->label21->Location = System::Drawing::Point(278, 87);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(596, 32);
			this->label21->TabIndex = 5;
			this->label21->Text = L"Вас приветствует программа учёта автомобилей\r\n";
			// 
			// MyForm2
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(1000, 650);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->outputTextBox);
			this->Controls->Add(this->but_exit);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->panel_report);
			this->Controls->Add(this->panel_look);
			this->Controls->Add(this->panel_manage);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label21);
			this->ForeColor = System::Drawing::Color::White;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm2";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"TRprogram";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->panel_manage->ResumeLayout(false);
			this->panel_manage->PerformLayout();
			this->panel_del->ResumeLayout(false);
			this->panel_del->PerformLayout();
			this->panel_change->ResumeLayout(false);
			this->panel_change->PerformLayout();
			this->panel_add->ResumeLayout(false);
			this->panel_add->PerformLayout();
			this->panel_look->ResumeLayout(false);
			this->panel_look->PerformLayout();
			this->panel_report->ResumeLayout(false);
			this->panel_report->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}
	private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {
		panel_manage->Hide();
	}
	private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e) {
		SidePanel->Height = but_manage->Height;
		SidePanel->Top = but_manage->Top;
		panel_manage->Show();
		panel_look->Hide();
		panel_report->Hide();
	}
	private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) {
		panel_del->Hide();
		panel_change->Hide();
		panel_add->Show();
	}
	private: System::Void button8_Click(System::Object^ sender, System::EventArgs^ e) {
		panel_add->Hide();
		panel_del->Hide();
		panel_change->Show();
	}
	private: System::Void button7_Click(System::Object^ sender, System::EventArgs^ e) {
		panel_add->Hide();		
		panel_change->Hide();
		panel_del->Show();
	}
	private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
		SidePanel->Height = button4->Height;
		SidePanel->Top = button4->Top;
		panel_look->Show();
		panel_manage->Hide();
		panel_report->Hide();
	}
	private: System::Void button15_Click(System::Object^ sender, System::EventArgs^ e) {
		panel_look->Hide();
	}
	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		SidePanel->Height = button3->Height;
		SidePanel->Top = button3->Top;
		panel_report->Show();
		panel_look->Hide();
		panel_manage->Hide();
	}
	private: System::Void button16_Click(System::Object^ sender, System::EventArgs^ e) {
		panel_report->Hide();
	}
private: System::Void button_add_Click(System::Object^ sender, System::EventArgs^ e) {
	// Получаем данные из текстовых полей
	String^ brand = textBox2->Text;
	String^ size = textBox5->Text;
	String^ owner = textBox4->Text;
	String^ carNumber = textBox3->Text;
	String^ weightWithCargo = textBox6->Text;
	String^ weightWithoutCargo = textBox7->Text;

	// Проверяем корректность введенных данных
	String^ validationErrors = ValidateInputData(brand, size, owner, carNumber, weightWithCargo, weightWithoutCargo);

	// Если есть ошибки, выводим их в поле вывода
	if (validationErrors != "") {
		outputTextBox->AppendText(validationErrors + "\r\n");
		return;
	}

	// Проверяем, существует ли уже автомобиль с указанным номером в базе
	String^ fileName = "records.bin";
	if (IsCarNumberExists(fileName, carNumber)) {
		outputTextBox->AppendText("Ошибка: Автомобиль с таким номером уже существует в базе.\r\n");
		return;
	}

	// Создаем новый объект Record и заполняем его данными из TextBox'ов
	Record newRecord;
	newRecord.Model = brand;
	newRecord.Size = size;
	newRecord.Name = owner;
	newRecord.Number = carNumber;
	newRecord.WeightWithCargo = weightWithCargo;
	newRecord.WeightWithoutCargo = weightWithoutCargo;

	// Вызываем функцию для добавления записи в файл
	AddRecord(fileName, newRecord);

	// Очищаем TextBox'ы после добавления записи
	textBox7->Clear();
	textBox2->Clear();
	textBox3->Clear();
	textBox4->Clear();
	textBox5->Clear();
	textBox6->Clear();
}
private: System::Void button14_Click(System::Object^ sender, System::EventArgs^ e) {
	String^ fileName = "records.bin"; // Имя файла
	DisplayRecords(fileName, outputTextBox);
}
private: System::Void button11_Click(System::Object^ sender, System::EventArgs^ e) {
	String^ carNumberToDelete = textBox18->Text; // Получаем номер автомобиля для удаления
	String^ fileName = "records.bin"; // Имя файла

	// Проверяем корректность формата номера автомобиля
	if (!IsValidCarNumber(carNumberToDelete)) {
		outputTextBox->AppendText("Неправильный формат номера автомобиля.\r\n");
		return;
	}

	RemoveRecordByCarNumber(fileName, carNumberToDelete, outputTextBox);
	textBox18 -> Clear();
}
private: System::Void button10_Click(System::Object^ sender, System::EventArgs^ e) {
	String^ ownerNameToUpdate = textBox11->Text; // Получаем ФИО владельца для обновления
	String^ carNumberToUpdate = textBox12->Text; // Получаем номер автомобиля для обновления
	String^ newBrand = textBox13->Text; // Получаем новую марку
	String^ newsize = textBox10->Text; // Получаем новый объем
	String^ newWeightWithCargo = textBox9->Text; // Получаем новую массу с грузом
	String^ newWeightWithoutCargo = textBox8->Text; // Получаем новую массу без груза
	String^ fileName = "records.bin"; // Имя файла

	// Проверяем введенные данные на корректность
	String^ errorMessage = ValidateInputData(newBrand, newsize, ownerNameToUpdate, carNumberToUpdate, newWeightWithCargo, newWeightWithoutCargo);
	if (errorMessage != "") {
		outputTextBox->AppendText(errorMessage + "\r\n");
		return;
	}

	// Проверяем существование записи с указанным ФИО и номером автомобиля
	if (!IsRecordExistsByOwnerAndCarNumber(fileName, ownerNameToUpdate, carNumberToUpdate)) {
		outputTextBox->AppendText("Запись с указанным ФИО и номером автомобиля не найдена.\r\n");
		return;
	}

	// Проверяем, уверен ли пользователь в изменении записи
	String^ confirmMessage = "Вы уверены, что хотите изменить запись?";
	String^ confirmCaption = "Подтверждение изменения";
	MessageBoxButtons confirmButtons = MessageBoxButtons::YesNo;
	MessageBoxIcon confirmIcon = MessageBoxIcon::Question;

	System::Windows::Forms::DialogResult confirmResult = MessageBox::Show(confirmMessage, confirmCaption, confirmButtons, confirmIcon);

	if (confirmResult == System::Windows::Forms::DialogResult::Yes) {
		// Вызываем функцию для изменения записи
		UpdateRecordByOwnerAndCarNumber(fileName, ownerNameToUpdate, carNumberToUpdate, newBrand, newsize, newWeightWithCargo, newWeightWithoutCargo, outputTextBox);
		textBox11->Clear();
		textBox12->Clear();
		textBox13->Clear();
		textBox10->Clear();
		textBox9->Clear();
		textBox8->Clear();
	}
	else {
		outputTextBox->AppendText("Изменение отменено пользователем.\r\n");
	}
}
private: System::Void button12_Click(System::Object^ sender, System::EventArgs^ e) {
	String^ fileName = "records.bin"; // Имя файла
	Dictionary<String^, int>^ brandCounts = gcnew Dictionary<String^, int>(); // Словарь для хранения марок и их количества
	outputTextBox->Clear();

	// Проверяем существование файла
	if (!File::Exists(fileName)) {
		outputTextBox->AppendText("Файл не найден.\r\n");
		return;
	}

	// Открываем файл для чтения
	FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read);
	BinaryReader^ reader = gcnew BinaryReader(fs);
	outputTextBox->AppendText("Марки автомобилей и их количество:\r\n");
	outputTextBox->AppendText("-----------------------------------------\r\n");

	try {
		while (reader->BaseStream->Position < reader->BaseStream->Length) {
			// Читаем марку из файла
			String^ brand = reader->ReadString();

			// Если марка уже есть в словаре, увеличиваем счетчик
			if (brandCounts->ContainsKey(brand)) {
				brandCounts[brand]++;
			}
			// Иначе добавляем марку в словарь со счетчиком 1
			else {
				brandCounts->Add(brand, 1);
			}

			// Пропускаем остальные поля записи
			reader->ReadString(); // Объем
			reader->ReadString(); // ФИО
			reader->ReadString(); // Номер автомобиля
			reader->ReadString(); // Масса с грузом
			reader->ReadString(); // Масса без груза
		}
	}
	catch (...) {
		// В случае ошибки выводим сообщение
		outputTextBox->AppendText("Ошибка при чтении файла.\r\n");
	}
	finally {
		// Закрываем потоки
		reader->Close();
		fs->Close();
	}

	// Выводим марки и их количество в поле вывода
	for each (KeyValuePair<String^, int> ^ kvp in brandCounts) {
		outputTextBox->AppendText(kvp->Key + ": " + kvp->Value + "\r\n");
	}
}
private: System::Void button13_Click(System::Object^ sender, System::EventArgs^ e) {
	String^ ownerName = textBox14->Text; // Получаем введённое ФИО владельца автомобилей

	// Проверяем, существуют ли автомобили для введённого ФИО
	if (!AreCarsExistForOwner(ownerName)) {
		MessageBox::Show("Для введённого владельца автомобили отсутствуют.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return; // Прерываем выполнение метода, так как нет автомобилей для сохранения
	}

	String^ fileName = "records.bin"; // Имя файла с данными
	SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog(); // Диалоговое окно для выбора места сохранения файла

	// Настраиваем диалоговое окно
	saveFileDialog1->Filter = "Бинарные файлы (*.bin)|*.bin|Все файлы (*.*)|*.*";
	saveFileDialog1->Title = "Выберите место для сохранения файла";
	saveFileDialog1->FileName = "cars_by_owner.bin";

	// Проверяем, было ли введено имя владельца
	if (ownerName->Length == 0) {
		MessageBox::Show("Введите фамилию владельца", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	// Открываем диалоговое окно для выбора места сохранения файла
	if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		// Получаем путь к выбранному файлу
		String^ filePath = saveFileDialog1->FileName;

		// Открываем файл для записи в бинарном формате
		FileStream^ fs = gcnew FileStream(filePath, FileMode::Create);
		BinaryWriter^ writer = gcnew BinaryWriter(fs);

		// Проверяем существование файла с данными
		if (!File::Exists(fileName)) {
			outputTextBox->AppendText("Файл не найден.\r\n");
			writer->Close();
			return;
		}

		// Открываем файл с данными для чтения
		FileStream^ dataFs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read);
		BinaryReader^ reader = gcnew BinaryReader(dataFs);

		try {
			while (reader->BaseStream->Position < reader->BaseStream->Length) {
				String^ brand = reader->ReadString(); // Марка автомобиля
				String^ size = reader->ReadString(); // Объем кузова
				String^ savedOwnerName = reader->ReadString(); // Читаем ФИО владельца
				String^ carNumber = reader->ReadString(); // Читаем номер автомобиля

				// Если ФИО владельца совпадает с введенным, записываем информацию об автомобиле в файл
				if (savedOwnerName == ownerName) {
					String^ cargoWeight = reader->ReadString(); // Масса с грузом
					String^ weightWithoutCargo = reader->ReadString(); // Масса без груза

					// Записываем информацию об автомобиле в файл
					writer->Write(brand);
					writer->Write(size);
					writer->Write(savedOwnerName);
					writer->Write(carNumber);
					writer->Write(cargoWeight);
					writer->Write(weightWithoutCargo);
				}
				else {
					// Пропускаем остальные поля записи
					reader->ReadString(); // Масса с грузом
					reader->ReadString(); // Масса без груза
				}
			}
		}
		catch (...) {
			// В случае ошибки выводим сообщение
			textBox7->AppendText("Ошибка при чтении файла.\r\n");
		}
		finally {
			// Закрываем потоки
			reader->Close();
			dataFs->Close();
			writer->Close();
			fs->Close();
		}

		// Оповещаем пользователя об успешном сохранении файла
		MessageBox::Show("Файл успешно сохранен", "Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
}
private: System::Void button1_Click_2(System::Object^ sender, System::EventArgs^ e) {
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog(); // Диалоговое окно для выбора файла
	openFileDialog1->Filter = "Бинарные файлы (*.bin)|*.bin|Все файлы (*.*)|*.*";
	openFileDialog1->Title = "Выберите файл для открытия";

	// Открываем диалоговое окно для выбора файла
	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		String^ filePath = openFileDialog1->FileName; // Получаем путь к выбранному файлу

		// Открываем файл для чтения в бинарном формате
		FileStream^ fs = gcnew FileStream(filePath, FileMode::Open);
		BinaryReader^ reader = gcnew BinaryReader(fs);

		try {
			// Очищаем поле вывода перед выводом новых данных
			outputTextBox->Clear();

			// Читаем данные из файла и выводим их в поле вывода
			while (reader->BaseStream->Position < reader->BaseStream->Length) {
				String^ brand = reader->ReadString(); // Читаем марку автомобиля
				String^ size = reader->ReadString(); // Читаем объем кузова
				String^ ownerName = reader->ReadString(); // Читаем ФИО владельца
				String^ carNumber = reader->ReadString(); // Читаем номер автомобиля
				String^ cargoWeight = reader->ReadString(); // Читаем массу с грузом
				String^ weightWithoutCargo = reader->ReadString(); // Читаем массу без груза

				// Выводим данные в поле вывода
				outputTextBox->AppendText("Марка: " + brand + "\r\n");
				outputTextBox->AppendText("Объем кузова(м^3): " + size + "\r\n");
				outputTextBox->AppendText("ФИО владельца: " + ownerName + "\r\n");
				outputTextBox->AppendText("Номер автомобиля: " + carNumber + "\r\n");
				outputTextBox->AppendText("Масса с грузом(кг): " + cargoWeight + "\r\n");
				outputTextBox->AppendText("Масса без груза(кг): " + weightWithoutCargo + "\r\n");
				outputTextBox->AppendText("--------------------------\r\n");
			}
		}
		catch (...) {
			// В случае ошибки выводим сообщение
			outputTextBox->AppendText("Ошибка при чтении файла.\r\n");
		}
		finally {
			// Закрываем потоки
			reader->Close();
			fs->Close();
		}
	}
}
	   bool dragging;
	   Point offset;
private: System::Void panel1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	dragging = true;
	offset.X = e->X;
	offset.Y = e->Y;
}
private: System::Void panel1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	if (dragging) {
		Point currentScreenPosition = PointToScreen(Point(e->X, e->Y));
		Location = Point(currentScreenPosition.X - offset.X, currentScreenPosition.Y - offset.Y);

	}
}
private: System::Void panel1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	dragging = false;
}
private: System::Void button1_Click_3(System::Object^ sender, System::EventArgs^ e) {
	SidePanel->Height = button1->Height;
	SidePanel->Top = button1->Top;
	panel_manage->Hide();
	panel_look->Hide();
	panel_report->Hide();
}
};
}
