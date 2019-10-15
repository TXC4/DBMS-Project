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

string serverUsername = "Carlton";
string serverPassword = "313MainSt!";

mysqlx::Session mySession(mysqlx::SessionOption::HOST, "localhost",
	mysqlx::SessionOption::PORT, 33060,
	mysqlx::SessionOption::USER, serverUsername,
	mysqlx::SessionOption::PWD, serverPassword);

mysqlx::Schema hotel = mySession.getSchema("hotel");
mysqlx::Table login = hotel.getTable("login");
mysqlx::Table loginAttempt = hotel.getTable("loginAttempt");
mysqlx::Table guest = hotel.getTable("guest");

int loginWindow();
int userWindow();
Fl_Window* win = new Fl_Window(750, 500, "Login");


void connectToServer()
{
	// Connect to MySQL Server on a network machine
	mysqlx::Session mySession(mysqlx::SessionOption::HOST, "localhost",
		mysqlx::SessionOption::PORT, 33060,
		mysqlx::SessionOption::USER, serverUsername,
		mysqlx::SessionOption::PWD, serverPassword);

	mysqlx::Schema hotel = mySession.getSchema("hotel");
	mysqlx::Table login = hotel.getTable("login");
	mysqlx::Table loginAttempt = hotel.getTable("loginAttempt");
	mysqlx::Table guest = hotel.getTable("guest");

	//lists all databases on accessed server
	list<mysqlx::Schema> schemaList = mySession.getSchemas();
	for (mysqlx::Schema schema : schemaList)
	{
		cout << schema.getName() << endl;
	}

	//After connecting to the server login window opens
	loginWindow();
}

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

struct UserPage
{
	Fl_Button *getNamesByRoom;
	Fl_Box *getNamesByRoomText;

	Fl_Input *roomNum;
	char roomNumber[3];
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

void searchLogByRoom_cb(Fl_Widget* getNamesByRoom, void* p)
{
	UserPage* userPage = reinterpret_cast<UserPage*>(p);
	strcpy_s(userPage->roomNumber, userPage->roomNum->value());
	string x = userPage->roomNumber;
	string singleQuote = "'";
	string comma = ",";
	string closeParen = ")";
	string query = "name = '" + x + singleQuote;
	mysqlx::RowResult myResult = guest.select("name", "roomNum").where(query).execute();

	mysqlx::Row row = myResult.fetchOne();
	cout << row[0] << ", " << row[1] << endl;
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
	UserPage userPage;
	
	Fl_Window *win = new Fl_Window(750, 500, "User Page");
	win->begin();

	userPage.getNamesByRoomText = new Fl_Box(250, 0, 200, 200, "Get Names by Room Number");
	userPage.getNamesByRoom = new Fl_Button(300, 200, 50, 25, "OK");
	userPage.roomNum = new Fl_Input(360, 125, 50, 25);

	userPage.getNamesByRoom->callback(searchLogByRoom_cb);

	win->end();
	win->show();

	return Fl::run();
}




int main()
{
	connectToServer();
}