#pragma once
#include "MDIParent.h"
namespace FinalGroupProject {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace MySql::Data::MySqlClient;

    public ref class LoginForm : public System::Windows::Forms::Form
    {
    private:
        System::ComponentModel::Container^ components;
        TextBox^ txtEmail;
        TextBox^ txtPassword;
        Button^ btnLogin;
        Label^ lblEmail;
        Label^ lblPassword;
        Label^ lblTitle;

    public:
        LoginForm(void)
        {
            InitializeComponent();
        }

    protected:
        ~LoginForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->txtEmail = (gcnew System::Windows::Forms::TextBox());
            this->txtPassword = (gcnew System::Windows::Forms::TextBox());
            this->btnLogin = (gcnew System::Windows::Forms::Button());
            this->lblEmail = (gcnew System::Windows::Forms::Label());
            this->lblPassword = (gcnew System::Windows::Forms::Label());
            this->lblTitle = (gcnew System::Windows::Forms::Label());

            // Form properties
            this->Text = L"Login";
            this->Size = System::Drawing::Size(400, 300);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;

            // Title Label
            lblTitle->Text = L"University Management System";
            lblTitle->Location = System::Drawing::Point(80, 20);
            lblTitle->Size = System::Drawing::Size(250, 30);
            lblTitle->Font = gcnew System::Drawing::Font("Arial", 12, FontStyle::Bold);

            // Labels
            lblEmail->Text = L"Email:";
            lblEmail->Location = System::Drawing::Point(50, 80);
            lblEmail->Size = System::Drawing::Size(100, 20);

            lblPassword->Text = L"Password:";
            lblPassword->Location = System::Drawing::Point(50, 130);
            lblPassword->Size = System::Drawing::Size(100, 20);

            // TextBoxes
            txtEmail->Location = System::Drawing::Point(150, 80);
            txtEmail->Size = System::Drawing::Size(180, 20);

            txtPassword->Location = System::Drawing::Point(150, 130);
            txtPassword->Size = System::Drawing::Size(180, 20);
            txtPassword->PasswordChar = '*';

            // Login Button
            btnLogin->Text = L"Login";
            btnLogin->Location = System::Drawing::Point(150, 180);
            btnLogin->Size = System::Drawing::Size(100, 30);
            btnLogin->Click += gcnew System::EventHandler(this, &LoginForm::btnLogin_Click);

            // Add controls
            this->Controls->Add(lblTitle);
            this->Controls->Add(lblEmail);
            this->Controls->Add(lblPassword);
            this->Controls->Add(txtEmail);
            this->Controls->Add(txtPassword);
            this->Controls->Add(btnLogin);
        }

        void btnLogin_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try {
                String^ connString = "Server = 127.0.0.1; Database = university_db; Uid = icp; Pwd = ";
                MySqlConnection^ conn = gcnew MySqlConnection(connString);
                conn->Open();

                String^ sql = "SELECT user_id, role, password FROM users WHERE email = @email";
                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Parameters->AddWithValue("@email", txtEmail->Text);

                MySqlDataReader^ reader = cmd->ExecuteReader();

                if (reader->Read())
                {
                    String^ storedPassword = reader["password"]->ToString();
                    if (txtPassword->Text == storedPassword)
                    {
                        String^ role = reader["role"]->ToString();
                        int userID = Convert::ToInt32(reader["user_id"]);

                        MessageBox::Show("Login successful! Welcome " + role);
                        // Here you would navigate to the appropriate form based on role
                        this->Hide();
                        MDIParent^ mdiForm = gcnew MDIParent(role, userID);
                        mdiForm->Show();
                    }
                    else
                    {
                        MessageBox::Show("Invalid password!");
                    }
                }
                else
                {
                    MessageBox::Show("User not found!");
                }

                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error: " + ex->Message);
            }
        }
    };
}