#include <iostream>
#include <string>
#include<FL/Fl.h>
#include<FL/Fl_Box.h>
#include<FL/Fl_Window.h>
#include<FL/Fl_Input.H>
#include<FL/Fl_Output.H>
#include<FL/Fl_Button.H>
#include <mysqlx/xdevapi.h>

//Console will prompt you for your server user name and password
//gui login user is "user" and password is "pass"

using namespace std;

int loginWindow();
int userWindow();
Fl_Window* win = new Fl_Window(750, 500, "Login");


//Custom struct used to keep track of our text input fields and the text inside
struct LoginInfo
{
	//These are our login and create account buttons
	Fl_Button *Login;
	Fl_Button *CreateAcc;


	//These are our text boxes
	Fl_Box *welcome;
	Fl_Box *invalid;
	Fl_Box *loggedIn;

	//These are our two text input fields
	Fl_Input *Uname;
	Fl_Input *Pass;


	//These are the two string values of whatever text is in Uname and Pass currently
	//note that these are currently only going to be updated when the Login button callback is called
	char user[20];
	char pass[20];
};

struct userPage
{
	Fl_Button *getNamesByRoom;
	Fl_Box *getNamesByRoomText;

	Fl_Input *roomNum;
	int roomNumber[3];
};


void login_cb(Fl_Widget* Login, void* p)
{

	//Here, we are recasting the void parameter p that we passed in back into a LoginInfo object
	LoginInfo* attempt = reinterpret_cast<LoginInfo*>(p);

	//copies the string values from each input field into our string variables in LoginInfo attempt
	strcpy_s(attempt->user, attempt->Uname->value());
	strcpy_s(attempt->pass, attempt->Pass->value());

	//sample if just to check if the username and password are a valid combination
	if ((strcmp(attempt->user, "user") != 0) || (strcmp(attempt->pass, "pass")))
	{
		attempt->welcome->hide();
		attempt->invalid->show();
	}
	else {
		attempt->welcome->hide();
		attempt->invalid->hide();
		attempt->loggedIn->show();
		
		win->hide(); //close login window
		userWindow();  //open user window
	}

	std::cout << attempt->user << std::endl << attempt->pass << std::endl;
}

void createAcc_cb(Fl_Widget*, void*)
{
	std::cout << "Sorry. This feature is not yet available" << std::endl;
}

void queryInput_cb(Fl_Widget*, void*)
{
	cout << "query";
}

int loginWindow()
{
	LoginInfo attempt; // create an empty instance of our custom struct defined earlier
	//Fl_Window* win = new Fl_Window(750, 500, "Login");
	win->begin();
	attempt.welcome = new Fl_Box(250, 0, 200, 200, "Welcome! Please log in below");
	attempt.invalid = new Fl_Box(250, 0, 200, 200, "Invalid Login, please try again");
	attempt.loggedIn = new Fl_Box(250, 0, 200, 200, "Successfully Logged in, Welcome!");
	attempt.loggedIn->hide();
	attempt.invalid->hide();

	attempt.Login = new Fl_Button(300, 300, 50, 25, "Login");
	attempt.Login->callback(login_cb, &attempt);
	attempt.CreateAcc = new Fl_Button(400, 300, 115, 25, "Create Account");
	attempt.CreateAcc->callback(createAcc_cb);

	attempt.Uname = new Fl_Input(300, 200, 215, 25, "Username:");
	//Uname.callback(Uname_cb);
	attempt.Pass = new Fl_Input(300, 250, 215, 25, "Password");
	//Pass.callback(Pass_cb);
	win->end();
	win->show();

	return Fl::run();
}

int userWindow()
{
	userPage attempt;
	Fl_Window *win = new Fl_Window(750, 500, "User Page");
	win->begin();

	attempt.getNamesByRoomText = new Fl_Box(250, 0, 200, 200, "Get Names by Room Number");
	attempt.getNamesByRoom = new Fl_Button(300, 150, 50, 25, "OK");

	attempt.getNamesByRoom->callback(queryInput_cb);

	win->end();
	win->show();

	return Fl::run();
}

string promptUserName()
{
	string username;
	cout << "Enter user name and press enter" << endl;
	cin >> username;
	return username;
}

string promptPassword()
{
	string password;
	cout << "Enter password and press enter" << endl;
	cin >> password;
	return password;
}

//TODO currently with failed login to local server
//you will get a program-ending error
void connectToServer()
{
	string username = promptUserName();
	string password = promptPassword();

	string usr = username;
	string pwd = password;

	// Connect to MySQL Server on a network machine
	mysqlx::Session mySession(mysqlx::SessionOption::HOST, "localhost",
		mysqlx::SessionOption::PORT, 33060,
		mysqlx::SessionOption::USER, usr,
		mysqlx::SessionOption::PWD, pwd);

	//lists all databases on accessed server
	list<mysqlx::Schema> schemaList = mySession.getSchemas();
	for (mysqlx::Schema schema : schemaList)
	{
		cout << schema.getName() << endl;
	}

	//make this as callback on button click
	//selects database to use and prints first 2 rows
	/*
	mySession.sql("use mydb;").execute();
	auto result = mySession.sql("select * from test;").execute();
	mysqlx::Row row = result.fetchOne();
	mysqlx::Row row2 = result.fetchOne();
	cout << row[0] << "," << row[1] << endl;
	cout << row2[0] << "," << row2[1] << endl;
	*/

	//After connecting to the server login window opens
	loginWindow();
}

int main()
{
	connectToServer();
}