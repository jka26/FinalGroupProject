#pragma once

namespace FinalGroupProject {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace MySql::Data::MySqlClient;
    using namespace System::Data;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    
    public ref class EnrollmentForm : public System::Windows::Forms::Form {
    private:
        System::ComponentModel::Container^ components;
        DataGridView^ dgvAvailableCourses;
        DataGridView^ dgvEnrolledCourses;
        Button^ btnEnroll;
        Button^ btnWithdraw;
        Label^ lblAvailable;
        Label^ lblEnrolled;
        int studentID;
        String^ connString;

    public:
        EnrollmentForm(int userID)
        {
            studentID = GetStudentID(userID);
            connString = "Server=localhost;Database=university_db;Uid=icp;Pwd=;";
            InitializeComponent();
            LoadAvailableCourses();
            LoadEnrolledCourses();
        }

    private:
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Text = L"Course Enrollment";
            this->Size = System::Drawing::Size(800, 600);

            lblAvailable = gcnew Label();
            lblAvailable->Text = "Available Courses";
            lblAvailable->Location = System::Drawing::Point(20, 20);
            lblAvailable->Size = System::Drawing::Size(200, 20);

            dgvAvailableCourses = gcnew DataGridView();
            dgvAvailableCourses->Location = System::Drawing::Point(20, 50);
            dgvAvailableCourses->Size = System::Drawing::Size(350, 200);
            dgvAvailableCourses->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgvAvailableCourses->MultiSelect = false;
            dgvAvailableCourses->ReadOnly = true;

            lblEnrolled = gcnew Label();
            lblEnrolled->Text = "Enrolled Courses";
            lblEnrolled->Location = System::Drawing::Point(20, 280);
            lblEnrolled->Size = System::Drawing::Size(200, 20);

            dgvEnrolledCourses = gcnew DataGridView();
            dgvEnrolledCourses->Location = System::Drawing::Point(20, 310);
            dgvEnrolledCourses->Size = System::Drawing::Size(350, 200);
            dgvEnrolledCourses->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgvEnrolledCourses->MultiSelect = false;
            dgvEnrolledCourses->ReadOnly = true;

            btnEnroll = gcnew Button();
            btnEnroll->Text = "Enroll";
            btnEnroll->Location = System::Drawing::Point(400, 120);
            btnEnroll->Size = System::Drawing::Size(100, 30);
            btnEnroll->Click += gcnew EventHandler(this, &EnrollmentForm::btnEnroll_Click);

            btnWithdraw = gcnew Button();
            btnWithdraw->Text = "Withdraw";
            btnWithdraw->Location = System::Drawing::Point(400, 380);
            btnWithdraw->Size = System::Drawing::Size(100, 30);
            btnWithdraw->Click += gcnew EventHandler(this, &EnrollmentForm::btnWithdraw_Click);

            this->Controls->Add(lblAvailable);
            this->Controls->Add(dgvAvailableCourses);
            this->Controls->Add(lblEnrolled);
            this->Controls->Add(dgvEnrolledCourses);
            this->Controls->Add(btnEnroll);
            this->Controls->Add(btnWithdraw);
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

        void LoadAvailableCourses()
        {
            MySqlConnection^ conn = gcnew MySqlConnection(connString);
            try {
                conn->Open();
                String^ sql = "SELECT c.courseID, c.courseName, s.Day, s.StartTime, s.EndTime, s.CourseLecturer " +
                    "FROM courses c " +
                    "JOIN schedule s ON c.courseID = s.CourseID " +
                    "WHERE c.courseID NOT IN " +
                    "(SELECT courseID FROM studentenrolmentdetails WHERE studentID = @studentID)";

                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Parameters->AddWithValue("@studentID", studentID);

                MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(cmd);
                DataTable^ table = gcnew DataTable();
                adapter->Fill(table);

                dgvAvailableCourses->DataSource = table;
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
                String^ sql = "SELECT c.courseID, c.courseName, s.Day, s.StartTime, s.EndTime, s.CourseLecturer " +
                    "FROM courses c " +
                    "JOIN schedule s ON c.courseID = s.CourseID " +
                    "JOIN studentenrolmentdetails sed ON c.courseID = sed.courseID " +
                    "WHERE sed.studentID = @studentID";

                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Parameters->AddWithValue("@studentID", studentID);

                MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(cmd);
                DataTable^ table = gcnew DataTable();
                adapter->Fill(table);

                dgvEnrolledCourses->DataSource = table;
            }
            finally {
                conn->Close();
            }
        }

        void btnEnroll_Click(Object^ sender, EventArgs^ e)
        {
            if (dgvAvailableCourses->SelectedRows->Count == 0) {
                MessageBox::Show("Please select a course to enroll in.");
                return;
            }

            MySqlConnection^ conn = gcnew MySqlConnection(connString);
            MySqlTransaction^ transaction = nullptr;

            try {
                conn->Open();
                transaction = conn->BeginTransaction();

                // Create enrollment record
                String^ sql = "INSERT INTO enrolment (Courses, enrolmentDate) VALUES (@courseName, NOW())";
                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Transaction = transaction;
                cmd->Parameters->AddWithValue("@courseName", dgvAvailableCourses->SelectedRows[0]->Cells["courseName"]->Value->ToString());
                cmd->ExecuteNonQuery();

                // Get the enrollment ID
                cmd->CommandText = "SELECT LAST_INSERT_ID()";
                int enrollmentID = Convert::ToInt32(cmd->ExecuteScalar());

                // Create enrollment details
                sql = "INSERT INTO studentenrolmentdetails (enrolmentID, studentID, courseID) VALUES (@enrollmentID, @studentID, @courseID)";
                cmd = gcnew MySqlCommand(sql, conn);
                cmd->Transaction = transaction;
                cmd->Parameters->AddWithValue("@enrollmentID", enrollmentID);
                cmd->Parameters->AddWithValue("@studentID", studentID);
                cmd->Parameters->AddWithValue("@courseID", dgvAvailableCourses->SelectedRows[0]->Cells["courseID"]->Value);
                cmd->ExecuteNonQuery();

                transaction->Commit();
                MessageBox::Show("Successfully enrolled in course!");

                LoadAvailableCourses();
                LoadEnrolledCourses();
            }
            catch (Exception^ ex) {
                if (transaction != nullptr) transaction->Rollback();
                MessageBox::Show("Error enrolling in course: " + ex->Message);
            }
            finally {
                conn->Close();
            }
        }

        void btnWithdraw_Click(Object^ sender, EventArgs^ e)
        {
            if (dgvEnrolledCourses->SelectedRows->Count == 0) {
                MessageBox::Show("Please select a course to withdraw from.");
                return;
            }

            MySqlConnection^ conn = gcnew MySqlConnection(connString);
            MySqlTransaction^ transaction = nullptr;

            try {
                conn->Open();
                transaction = conn->BeginTransaction();

                int courseID = Convert::ToInt32(dgvEnrolledCourses->SelectedRows[0]->Cells["courseID"]->Value);

                // Get enrollmentID
                String^ sql = "SELECT enrolmentID FROM studentenrolmentdetails WHERE studentID = @studentID AND courseID = @courseID";
                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Transaction = transaction;
                cmd->Parameters->AddWithValue("@studentID", studentID);
                cmd->Parameters->AddWithValue("@courseID", courseID);
                Object^ result = cmd->ExecuteScalar();
                int enrollmentID = Convert::ToInt32(result);

                // Delete enrollment details
                sql = "DELETE FROM studentenrolmentdetails WHERE enrolmentID = @enrollmentID";
                cmd = gcnew MySqlCommand(sql, conn);
                cmd->Transaction = transaction;
                cmd->Parameters->AddWithValue("@enrollmentID", enrollmentID);
                cmd->ExecuteNonQuery();

                // Delete enrollment
                sql = "DELETE FROM enrolment WHERE enrolmentID = @enrollmentID";
                cmd = gcnew MySqlCommand(sql, conn);
                cmd->Transaction = transaction;
                cmd->Parameters->AddWithValue("@enrollmentID", enrollmentID);
                cmd->ExecuteNonQuery();

                transaction->Commit();
                MessageBox::Show("Successfully withdrawn from course!");

                LoadAvailableCourses();
                LoadEnrolledCourses();
            }
            catch (Exception^ ex) {
                if (transaction != nullptr) transaction->Rollback();
                MessageBox::Show("Error withdrawing from course: " + ex->Message);
            }
            finally {
                conn->Close();
            }
        }
    };
}