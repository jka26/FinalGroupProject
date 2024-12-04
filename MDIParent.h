#pragma once
#include "EnrollmentForm.h"
#include "ViewGradesForm.h"
#include "ViewProfileForm.h"
namespace FinalGroupProject {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace MySql::Data::MySqlClient;

    public ref class MDIParent : public System::Windows::Forms::Form
    {
    private:
        System::ComponentModel::Container^ components;
        MenuStrip^ menuStrip;
        ToolStripMenuItem^ fileMenu;
        ToolStripMenuItem^ studentMenu;
        ToolStripMenuItem^ facultyMenu;
        ToolStripMenuItem^ adminMenu;
        ToolStripMenuItem^ logoutMenuItem;

        String^ userRole;
        int userID;

    public:
        MDIParent(String^ role, int id)
        {
            userRole = role;
            userID = id;
            InitializeComponent();
        }

    protected:
        ~MDIParent()
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
            this->menuStrip = gcnew MenuStrip();

            // Configure MDI Form
            this->IsMdiContainer = true;
            this->WindowState = FormWindowState::Maximized;
            this->Text = L"University Management System";

            // File Menu
            fileMenu = gcnew ToolStripMenuItem("File");
            logoutMenuItem = gcnew ToolStripMenuItem("Logout");
            logoutMenuItem->Click += gcnew EventHandler(this, &MDIParent::LogoutMenuItem_Click);
            fileMenu->DropDownItems->Add(logoutMenuItem);

            // Student Menu
            studentMenu = gcnew ToolStripMenuItem("Student");
            studentMenu->DropDownItems->Add("View Schedule", nullptr, gcnew EventHandler(this, &MDIParent::ViewSchedule_Click));
            studentMenu->DropDownItems->Add("Enroll in Courses", nullptr, gcnew EventHandler(this, &MDIParent::EnrollCourses_Click));
            studentMenu->DropDownItems->Add("View Grades", nullptr, gcnew EventHandler(this, &MDIParent::ViewGrades_Click));
            studentMenu->DropDownItems->Add("View Profile", nullptr, gcnew EventHandler(this, &MDIParent::ViewProfile_Click));

            // Faculty Menu
            facultyMenu = gcnew ToolStripMenuItem("Faculty");
            facultyMenu->DropDownItems->Add("Manage Courses", nullptr, gcnew EventHandler(this, &MDIParent::ManageCourses_Click));
            facultyMenu->DropDownItems->Add("Enter Grades", nullptr, gcnew EventHandler(this, &MDIParent::EnterGrades_Click));
            facultyMenu->DropDownItems->Add("View Class Roster", nullptr, gcnew EventHandler(this, &MDIParent::ViewRoster_Click));
            facultyMenu->DropDownItems->Add("Update Profile", nullptr, gcnew EventHandler(this, &MDIParent::UpdateProfile_Click));

            // Admin Menu
            adminMenu = gcnew ToolStripMenuItem("Admin");
            adminMenu->DropDownItems->Add("Manage Students", nullptr, gcnew EventHandler(this, &MDIParent::ManageStudents_Click));
            adminMenu->DropDownItems->Add("Manage Faculty", nullptr, gcnew EventHandler(this, &MDIParent::ManageFaculty_Click));
            adminMenu->DropDownItems->Add("Generate Reports", nullptr, gcnew EventHandler(this, &MDIParent::GenerateReports_Click));

            // Add menus to strip
            menuStrip->Items->Add(fileMenu);

            // Add role-specific menus
            if (userRole == "Student") {
                menuStrip->Items->Add(studentMenu);
            }
            else if (userRole == "Faculty") {
                menuStrip->Items->Add(facultyMenu);
            }
            else if (userRole == "Admin") {
                menuStrip->Items->Add(studentMenu);
                menuStrip->Items->Add(facultyMenu);
                menuStrip->Items->Add(adminMenu);
            }

            this->MainMenuStrip = menuStrip;
            this->Controls->Add(menuStrip);
        }

        // Menu click handlers
        void LogoutMenuItem_Click(Object^ sender, EventArgs^ e) {
            this->Close();
            Application::Restart();
        }

        void ViewSchedule_Click(Object^ sender, EventArgs^ e) {
            // Create and show schedule form
            // Implementation here
        }

        void EnrollCourses_Click(Object^ sender, EventArgs^ e) {
            EnrollmentForm^ enrollForm = gcnew EnrollmentForm(userID);
            enrollForm->MdiParent = this;
            enrollForm->Show();
        }

        void ViewGrades_Click(Object^ sender, EventArgs^ e) {
            ViewGradesForm^ viewGrades = gcnew ViewGradesForm(userID);
            viewGrades->MdiParent = this;
            viewGrades->Show();
        }

        void ViewProfile_Click(Object^ sender, EventArgs^ e) {
            ViewProfileForm^ profileForm = gcnew ViewProfileForm(userID);
            profileForm->MdiParent = this;
            profileForm->Show();
        }

        void ManageCourses_Click(Object^ sender, EventArgs^ e) {
            // Create and show course management form
            // Implementation here
        }

        void EnterGrades_Click(Object^ sender, EventArgs^ e) {
            // Create and show grade entry form
            // Implementation here
        }

        void ViewRoster_Click(Object^ sender, EventArgs^ e) {
            // Create and show class roster form
            // Implementation here
        }

        void UpdateProfile_Click(Object^ sender, EventArgs^ e) {
            // Create and show profile update form
            // Implementation here
        }

        void ManageStudents_Click(Object^ sender, EventArgs^ e) {
            // Create and show student management form
            // Implementation here
        }

        void ManageFaculty_Click(Object^ sender, EventArgs^ e) {
            // Create and show faculty management form
            // Implementation here
        }

        void GenerateReports_Click(Object^ sender, EventArgs^ e) {
            // Create and show reports form
            // Implementation here
        }
    };
}