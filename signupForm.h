#pragma once
#include "LoginForm.h"
namespace FinalGroupProject {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace MySql::Data::MySqlClient;

    public ref class SignupForm : public System::Windows::Forms::Form
    {
    private:
        // Form components
        System::ComponentModel::Container^ components;
        TextBox^ txtFirstName;
        TextBox^ txtLastName;
        TextBox^ txtEmail;
        TextBox^ txtPassword;
        ComboBox^ cboRole;
        Button^ btnSignup;
        Label^ lblFirstName;
        Label^ lblLastName;
        Label^ lblEmail;
        Label^ lblPassword;
        Label^ lblRole;
        LinkLabel^ lnkLogin;

    public:
        SignupForm(void)
        {
            InitializeComponent();
        }

    protected:
        ~SignupForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        void InitializeComponent(void)
        {
            this->txtFirstName = (gcnew System::Windows::Forms::TextBox());
            this->txtLastName = (gcnew System::Windows::Forms::TextBox());
            this->txtEmail = (gcnew System::Windows::Forms::TextBox());
            this->txtPassword = (gcnew System::Windows::Forms::TextBox());
            this->cboRole = (gcnew System::Windows::Forms::ComboBox());
            this->btnSignup = (gcnew System::Windows::Forms::Button());
            this->lblFirstName = (gcnew System::Windows::Forms::Label());
            this->lblLastName = (gcnew System::Windows::Forms::Label());
            this->lblEmail = (gcnew System::Windows::Forms::Label());
            this->lblPassword = (gcnew System::Windows::Forms::Label());
            this->lblRole = (gcnew System::Windows::Forms::Label());

            // Form properties
            this->Text = L"User Registration";
            this->Size = System::Drawing::Size(400, 350);
            this->StartPosition = FormStartPosition::CenterScreen;

            // Labels
            lblFirstName->Text = L"First Name:";
            lblFirstName->Location = System::Drawing::Point(30, 30);
            lblFirstName->Size = System::Drawing::Size(100, 20);

            lblLastName->Text = L"Last Name:";
            lblLastName->Location = System::Drawing::Point(30, 80);
            lblLastName->Size = System::Drawing::Size(100, 20);

            lblEmail->Text = L"Email:";
            lblEmail->Location = System::Drawing::Point(30, 130);
            lblEmail->Size = System::Drawing::Size(100, 20);

            lblPassword->Text = L"Password:";
            lblPassword->Location = System::Drawing::Point(30, 180);
            lblPassword->Size = System::Drawing::Size(100, 20);

            lblRole->Text = L"Role:";
            lblRole->Location = System::Drawing::Point(30, 230);
            lblRole->Size = System::Drawing::Size(100, 20);

            // TextBoxes
            txtFirstName->Location = System::Drawing::Point(150, 30);
            txtFirstName->Size = System::Drawing::Size(200, 20);

            txtLastName->Location = System::Drawing::Point(150, 80);
            txtLastName->Size = System::Drawing::Size(200, 20);

            txtEmail->Location = System::Drawing::Point(150, 130);
            txtEmail->Size = System::Drawing::Size(200, 20);

            txtPassword->Location = System::Drawing::Point(150, 180);
            txtPassword->Size = System::Drawing::Size(200, 20);
            txtPassword->PasswordChar = '*';

            // ComboBox
            cboRole->Location = System::Drawing::Point(150, 230);
            cboRole->Size = System::Drawing::Size(200, 20);
            cboRole->Items->Add("Student");
            cboRole->Items->Add("Faculty");
            cboRole->DropDownStyle = ComboBoxStyle::DropDownList;

            // Button
            btnSignup->Text = L"Sign Up";
            btnSignup->Location = System::Drawing::Point(150, 280);
            btnSignup->Size = System::Drawing::Size(100, 30);
            btnSignup->Click += gcnew System::EventHandler(this, &SignupForm::btnSignup_Click);

            this->lnkLogin = (gcnew System::Windows::Forms::LinkLabel());
            lnkLogin->Text = L"Already have an account?";
            lnkLogin->Location = System::Drawing::Point(150, 320);
            lnkLogin->Size = System::Drawing::Size(150, 20);
            lnkLogin->Click += gcnew System::EventHandler(this, &SignupForm::lnkLogin_Click);

            // Add controls to form
            this->Controls->Add(lblFirstName);
            this->Controls->Add(lblLastName);
            this->Controls->Add(lblEmail);
            this->Controls->Add(lblPassword);
            this->Controls->Add(lblRole);
            this->Controls->Add(txtFirstName);
            this->Controls->Add(txtLastName);
            this->Controls->Add(txtEmail);
            this->Controls->Add(txtPassword);
            this->Controls->Add(cboRole);
            this->Controls->Add(btnSignup);
            this->Controls->Add(lnkLogin);
        }

        void btnSignup_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try {
                String^ connString = "Server=localhost;Database=university_db;Uid=icp;Pwd=";
                MySqlConnection^ conn = gcnew MySqlConnection(connString);
                conn->Open();

                // Insert into users table
                String^ sql = "INSERT INTO users (first_name, last_name, email, password, role) VALUES (@fname, @lname, @email, @pwd, @role)";
                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Parameters->AddWithValue("@fname", txtFirstName->Text);
                cmd->Parameters->AddWithValue("@lname", txtLastName->Text);
                cmd->Parameters->AddWithValue("@email", txtEmail->Text);
                cmd->Parameters->AddWithValue("@pwd", txtPassword->Text);
                cmd->Parameters->AddWithValue("@role", cboRole->SelectedItem->ToString());

                cmd->ExecuteNonQuery();

                // Get the last inserted user_id
                sql = "SELECT LAST_INSERT_ID()";
                cmd = gcnew MySqlCommand(sql, conn);
                int userID = Convert::ToInt32(cmd->ExecuteScalar());

                // Insert into respective role table (student or faculty)
                if (cboRole->SelectedItem->ToString() == "Student") {
                    sql = "INSERT INTO students (userID, Studentfname, Studentlname, email) VALUES (@uid, @fname, @lname, @email)";
                }
                else {
                    sql = "INSERT INTO faculty (userID, facultyfName, facultylname, email) VALUES (@uid, @fname, @lname, @email)";
                }

                cmd = gcnew MySqlCommand(sql, conn);
                cmd->Parameters->AddWithValue("@uid", userID);
                cmd->Parameters->AddWithValue("@fname", txtFirstName->Text);
                cmd->Parameters->AddWithValue("@lname", txtLastName->Text);
                cmd->Parameters->AddWithValue("@email", txtEmail->Text);

                cmd->ExecuteNonQuery();
                conn->Close();

                MessageBox::Show("Registration successful!");
                this->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error: " + ex->Message);
            }
        }

        void lnkLogin_Click(System::Object^ sender, System::EventArgs^ e)
        {
            this->Hide();
            LoginForm^ loginForm = gcnew LoginForm();
            loginForm->Show();
        }
    };
}