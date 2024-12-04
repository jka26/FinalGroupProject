#pragma once

namespace FinalGroupProject {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace MySql::Data::MySqlClient;

    public ref class ViewProfileForm : public System::Windows::Forms::Form {
    private:
        System::ComponentModel::Container^ components;
        Label^ lblTitle;
        Label^ lblName;
        Label^ lblEmail;
        DataGridView^ dgvCourses;
        int studentID;
        String^ connString;

    public:
        ViewProfileForm(int userID)
        {
            studentID = GetStudentID(userID);
            connString = "Server=localhost;Database=university_db;Uid=icp;Pwd=;";
            InitializeComponent();
            LoadStudentProfile();
            LoadEnrolledCourses();
        }

    private:
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Text = L"Student Profile";
            this->Size = System::Drawing::Size(800, 500);

            // Title
            lblTitle = gcnew Label();
            lblTitle->Text = L"My Profile";
            lblTitle->Location = System::Drawing::Point(20, 20);
            lblTitle->Size = System::Drawing::Size(200, 30);
            lblTitle->Font = gcnew System::Drawing::Font("Arial", 12, System::Drawing::FontStyle::Bold);

            // Student Name
            lblName = gcnew Label();
            lblName->Location = System::Drawing::Point(20, 60);
            lblName->Size = System::Drawing::Size(400, 20);
            lblName->Font = gcnew System::Drawing::Font("Arial", 10);

            // Student Email
            lblEmail = gcnew Label();
            lblEmail->Location = System::Drawing::Point(20, 90);
            lblEmail->Size = System::Drawing::Size(400, 20);
            lblEmail->Font = gcnew System::Drawing::Font("Arial", 10);

            // Enrolled Courses Grid
            dgvCourses = gcnew DataGridView();
            dgvCourses->Location = System::Drawing::Point(20, 130);
            dgvCourses->Size = System::Drawing::Size(740, 300);
            dgvCourses->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgvCourses->ReadOnly = true;
            dgvCourses->AllowUserToAddRows = false;
            dgvCourses->AllowUserToDeleteRows = false;
            dgvCourses->AllowUserToOrderColumns = true;
            dgvCourses->RowHeadersVisible = false;
            dgvCourses->AlternatingRowsDefaultCellStyle->BackColor = System::Drawing::Color::LightGray;

            this->Controls->Add(lblTitle);
            this->Controls->Add(lblName);
            this->Controls->Add(lblEmail);
            this->Controls->Add(dgvCourses);
        }

        int GetStudentID(int userID)
        {
            MySqlConnection^ conn = gcnew MySqlConnection(connString);
            try {
                conn->Open();
                String^ sql = "SELECT studentID FROM students WHERE userID = @userID";
                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Parameters->AddWithValue("@userID", userID);
                Object^ result = cmd->ExecuteScalar();
                return Convert::ToInt32(result);
            }
            finally {
                conn->Close();
            }
        }

        void LoadStudentProfile()
        {
            MySqlConnection^ conn = gcnew MySqlConnection(connString);
            try {
                conn->Open();
                String^ sql = "SELECT s.Studentfname, s.Studentlname, s.email " +
                    "FROM students s " +
                    "WHERE s.studentID = @studentID";

                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Parameters->AddWithValue("@studentID", studentID);

                MySqlDataReader^ reader = cmd->ExecuteReader();
                if (reader->Read())
                {
                    String^ fullName = String::Format("{0} {1}",
                        reader["Studentfname"]->ToString(),
                        reader["Studentlname"]->ToString());
                    lblName->Text = "Name: " + fullName;
                    lblEmail->Text = "Email: " + reader["email"]->ToString();
                }
            }
            finally {
                conn->Close();
            }
        }

        void LoadEnrolledCourses()
        {
            MySqlConnection^ conn = gcnew MySqlConnection(connString);
            try {
                conn->Open();
                String^ sql = "SELECT DISTINCT c.courseName, s.CourseLecturer, s.Day, " +
                    "CONCAT(s.StartTime, ' - ', s.EndTime) as 'Class Time' " +
                    "FROM courses c " +
                    "JOIN studentenrolmentdetails sed ON c.courseID = sed.courseID " +
                    "JOIN schedule s ON c.courseID = s.CourseID " +
                    "WHERE sed.studentID = @studentID";

                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Parameters->AddWithValue("@studentID", studentID);

                MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(cmd);
                DataTable^ table = gcnew DataTable();
                adapter->Fill(table);

                dgvCourses->DataSource = table;

                // Format columns
                dgvCourses->Columns["courseName"]->HeaderText = "Course";
                dgvCourses->Columns["CourseLecturer"]->HeaderText = "Lecturer";
                dgvCourses->Columns["Day"]->HeaderText = "Day";
                dgvCourses->Columns["Class Time"]->HeaderText = "Time";
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error loading courses: " + ex->Message);
            }
            finally {
                conn->Close();
            }
        }
    };
}