#pragma once

namespace FinalGroupProject {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace MySql::Data::MySqlClient;
    using namespace System::Data;

    public ref class ViewGradesForm : public System::Windows::Forms::Form {
    private:
        System::ComponentModel::Container^ components;
        DataGridView^ dgvGrades;
        Label^ lblTitle;
        int studentID;
        String^ connString;

    public:
        ViewGradesForm(int userID)
        {
            studentID = GetStudentID(userID);
            connString = "Server=localhost;Database=university_db;Uid=icp;Pwd=;";
            InitializeComponent();
            LoadGrades();
        }

    private:
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Text = L"View Grades";
            this->Size = System::Drawing::Size(800, 500);

            lblTitle = gcnew Label();
            lblTitle->Text = L"My Academic Record";
            lblTitle->Location = System::Drawing::Point(20, 20);
            lblTitle->Size = System::Drawing::Size(200, 30);
            lblTitle->Font = gcnew System::Drawing::Font("Arial", 12, System::Drawing::FontStyle::Bold);

            dgvGrades = gcnew DataGridView();
            dgvGrades->Location = System::Drawing::Point(20, 60);
            dgvGrades->Size = System::Drawing::Size(740, 380);
            dgvGrades->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgvGrades->ReadOnly = true;
            dgvGrades->AllowUserToAddRows = false;
            dgvGrades->AllowUserToDeleteRows = false;
            dgvGrades->AllowUserToOrderColumns = true;
            dgvGrades->MultiSelect = false;
            dgvGrades->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgvGrades->RowHeadersVisible = false;
            dgvGrades->AlternatingRowsDefaultCellStyle->BackColor = System::Drawing::Color::LightGray;

            this->Controls->Add(lblTitle);
            this->Controls->Add(dgvGrades);
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

        void LoadGrades()
        {
            MySqlConnection^ conn = gcnew MySqlConnection(connString);
            try {
                conn->Open();
                String^ sql = "SELECT c.courseName, g.Grade, s.CourseLecturer, s.Day, "
                    "CONCAT(s.StartTime, ' - ', s.EndTime) as 'Class Time' "
                    "FROM grades g "
                    "JOIN courses c ON g.CourseID = c.courseID "
                    "JOIN schedule s ON c.courseID = s.CourseID "
                    "WHERE g.StudentID = @studentID";

                MySqlCommand^ cmd = gcnew MySqlCommand(sql, conn);
                cmd->Parameters->AddWithValue("@studentID", studentID);

                MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(cmd);
                DataTable^ table = gcnew DataTable();
                adapter->Fill(table);

                dgvGrades->DataSource = table;

                // Format columns
                dgvGrades->Columns["courseName"]->HeaderText = "Course Name";
                dgvGrades->Columns["Grade"]->HeaderText = "Grade";
                dgvGrades->Columns["CourseLecturer"]->HeaderText = "Lecturer";
                dgvGrades->Columns["Day"]->HeaderText = "Day";
                dgvGrades->Columns["Class Time"]->HeaderText = "Time";

                // Calculate GPA
                double totalPoints = 0;
                int totalCourses = table->Rows->Count;
                for each (DataRow ^ row in table->Rows)
                {
                    String^ grade = row["Grade"]->ToString();
                    if (grade == "A") totalPoints += 4.0;
                    else if (grade == "B+") totalPoints += 3.5;
                    else if (grade == "B") totalPoints += 3.0;
                    else if (grade == "C+") totalPoints += 2.5;
                    else if (grade == "C") totalPoints += 2.0;
                    else if (grade == "D+") totalPoints += 1.5;
                    else if (grade == "D") totalPoints += 1.0;
                    else if (grade == "F") totalPoints += 0.0;
                }

                if (totalCourses > 0)
                {
                    double gpa = totalPoints / totalCourses;
                    Label^ lblGPA = gcnew Label();
                    lblGPA->Text = String::Format("GPA: {0:F2}", gpa);
                    lblGPA->Location = System::Drawing::Point(600, 20);
                    lblGPA->Size = System::Drawing::Size(150, 30);
                    lblGPA->Font = gcnew System::Drawing::Font("Arial", 12, System::Drawing::FontStyle::Bold);
                    this->Controls->Add(lblGPA);
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Error loading grades: " + ex->Message);
            }
            finally {
                conn->Close();
            }
        }
    };
}