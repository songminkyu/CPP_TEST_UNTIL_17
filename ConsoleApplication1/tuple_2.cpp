
#include "stdafx.h" 
#include "tuple_2.h"

/**************************************************************************************/


#define main_active // 메인 활성화


// C 언어 전용 쓰레드 예제

// GetRandom returns a random integer between min and max.
#define GetRandom( min, max ) ((rand() % (int)(((max) + 1) - (min))) + (min))
// GetGlyph returns a printable ASCII character value
#define GetGlyph( val ) ((char)((val + 32) % 93 + 33))

#define MAX_THREADS 3 
#define BUF_SIZE 255 
#define MAX_THREADS_ (10)


DWORD STotal = 0;
BOOL repeat = TRUE;                 // Global repeat flag 
HANDLE hStdOut;                     // Handle for console window
CONSOLE_SCREEN_BUFFER_INFO csbi;    // Console information structure


void Bounce(void *);
void CheckKey(void *);

unsigned __stdcall SecondThreadFunc_1(void* pArguments);
unsigned __stdcall SecondThreadFunc_2(void* pArguments);
unsigned __stdcall SecondThreadFunc_3(void* pArguments);

unsigned int Window_CE_SecondThreadFunc_1(LPTHREAD_START_ROUTINE *);
unsigned int Window_CE_SecondThreadFunc_2(LPTHREAD_START_ROUTINE *);
unsigned int Window_CE_SecondThreadFunc_3(LPTHREAD_START_ROUTINE *);

DWORD WINAPI myThread(void* parameter);

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
void ErrorHandler(LPTSTR lpszFunction);	
void ErrorHandler(LPTSTR lpszFunction);

// 생성될 Thread가 수행할 내용이 담긴 함수.
// CreateThread와는 다르게 unsigned int 자료형을 사용합니다.
UINT WINAPI UINT_ThreadProc(LPVOID lpParam);

DWORD WINAPI DWORD_ThreadProc(LPVOID lpParam);

std::string literal_return(int type);
/**************************************************************************************/

void Thread_Tuple_Implement();
void component_Implement();
void C_type_beginthreadex();
void C_type_beginthread();
void variable_factor_function();
void C_type_beginthread_in_Muti();
void C_type_parameter_delivery_thread();
void C_type_parameter_delivery_Multi_thread();
void C_type_parameter_beginethread_thread();
void C_type_parameter_createthread_thread();
void CPP_type_parameter_delivery_thread();
void Chrono_time_check();


#ifdef main_active
void main(int argc, char* argv[])
{
	cout << "/************************** 튜플 예제 및 튜플을 이용한 쓰레드 구현부 ^_^ ********************************/" << endl;

	Thread_Tuple_Implement();
	
	cout << "/************************** Chrono ********************************/" << endl;

	Chrono_time_check();

	cout << "/************************** 가변인자 템플릿 ********************************/" << endl;

	variable_factor_function();

	cout << "/************************** 컴포넌트 관리********************************/" << endl;
	
	component_Implement();

	cout << "/************************** C언어 전용 싱글 쓰레드 ********************************/" << endl;

	C_type_beginthread();
	//Enter 입력시 다음 쓰레드로 넘어감.
	C_type_beginthreadex();

	C_type_parameter_delivery_thread();

	C_type_parameter_beginethread_thread();

	C_type_parameter_createthread_thread();

	cout << "/************************** C언어 전용 멀티 쓰레드 ********************************/" << endl;

	C_type_beginthread_in_Muti();

	C_type_parameter_delivery_Multi_thread();

	cout << "/************************** C언어 전용 싱글 쓰레드 함수 와 CPP 클래스를 조합한 함수 ********************************/" << endl;

	CPP_type_parameter_delivery_thread();

}
#endif

void Thread_Tuple_Implement()
{
	int i = 0;
	int int_resive = 1;
	int int_array[4] = { 192,168,2,1 };
	double double_resive = 1;
	double double_array[4] = { 100.7,667.2,70.4, 6.9 };

	TimeCheck time_check;
	deque<std::string> dq_str;
	vector<int>		   vt_int;
	vector<future<double>> vt_fure;
	list<double>	   ld_double;
	pair<std::complex<double>, std::string> pr_is(
		std::piecewise_construct,
		std::forward_as_tuple(0.123, 7.7),
		std::forward_as_tuple(10, 'a'));

	std::string str_array[100] = { "song","min","kyu" };
	std::string cs;
	std::string str;
	

	xtuple<string_set_class> just_only_user_class{ " have a good" };
	xtuple<
		deque<std::string>,
		vector<int>,
		list<double>,
		pair<std::complex<double>, std::string>,
		xtuple<string_set_class>,
		vector<future<double>>
	> t_a(dq_str, vt_int, ld_double, pr_is, just_only_user_class, vt_fure);

	for (UINT i = 0; i < str_array->size(); i++)						Get<0>(t_a).emplace_back(str_array[i]);
	for (UINT i = 0; i < sizeof(int_array) / sizeof(int); i++)			Get<1>(t_a).emplace_back(int_array[i]);
	for (UINT i = 0; i < sizeof(double_array) / sizeof(double); i++)	Get<2>(t_a).emplace_back(double_array[i]);

	auto &&add_string = Get<4>(t_a).first_.getter_string_set("day");

	for (auto && item : Get<0>(t_a))	cs += item;
	for (auto && item : Get<1>(t_a))    int_resive *= item;
	for (auto && item : Get<2>(t_a))    double_resive /= item;

	cout << cs + add_string << "\n" << int_resive << "\n" << double_resive << "\n" << endl; //< a
	cout << "pair : " << Get<3>(t_a).first << " , " << Get<3>(t_a).second << endl;

	auto a = song_make_tuple(5, "Hello", -0.1, "song");
	std::cout << a << std::endl; // prints: (5, "Hello", -0.1)
	//cout << get<0>(a) << " " << get<1>(a) << " " << get<2>(a) << " " << get<3>(a) << endl;

	auto b = std::make_tuple(-0.1, "song", 5, "Hello");
	std::cout << b << std::endl; // prints: (5, "Hello", -0.1)
	cout << get<0>(b) << " " << get<1>(b) << " " << get<2>(b) << " " << get<3>(b) << endl;


	/************************** Thread & Tuple Implement ********************************/

	/************************일반 함수 타입 ***********************/

	//cout << "/************************** Thread & Tuple Implement ********************************/" << endl;

	//double(*Getdata[])(double, double, double,std::string&) = {
	//	customer__fibonacci_make_function_start,
	//	customer__fibonacci_make_function_2,
	//	customer__fibonacci_make_function_3,
	//	customer__fibonacci_make_function_4,
	//	customer__fibonacci_make_function_5,
	//	customer__fibonacci_make_function_end };

	//for (int i = 0; i < 6; ++i)
	//{
	//	str = literal_return(i);
	//	Get<5>(t_a).push_back(async(Getdata[i], ((1 + i)  * 1.2), ((1 + i) * 3.4), ((1 + i) * 5.6), str));
	//}

	//for (auto& x : Get<5>(t_a))
	//{
	//	future <double> y = x.then(multply_than_make_function);
	//	future <double> z = x.then(divide_than_make_function);
	//	std::cout << y.get() << std::endl;
	//	std::cout << z.get() << std::endl;
	//	
	//}

	cout << "/************************** Thread & LAMBDA ********************************/" << endl;

	std::string str_;
	auto f_x = std::bind(customer__fibonacci_make_function_start, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	
	for (int i = 0; i != 10; ++i)
	{
		Get<5>(t_a).emplace_back(async(f_x, ((1 + i)  * 1.2), ((1 + i) * 3.4), ((1 + i) * 5.6), str_));
	}
	for (auto& x : Get<5>(t_a))
	{
		future <double> y = x.then(multply_than_make_function);
		future <double> z = x.then(divide_than_make_function);
		std::cout << y.get() << std::endl;
		std::cout << z.get() << std::endl;
	}

	cpp_int x;
	/*
	future<cpp_int> x = async([] { return fibonacci<cpp_int>(100); });

	future<cpp_int> y = x.then([](const cpp_int& x) { return cpp_int(x * 2); });
	future<cpp_int> z = x.then([](const cpp_int& x) { return cpp_int(x / 15); });

	cout << y.get() << endl;
	cout << z.get() << endl;*/
}

std::string literal_return(int type)
{
	std::string str;
	if		(type == 0) str = "RED";
	else if	(type == 1)	str = "ORANGE";
	else if (type == 2) str = "YELLOW";
	else if (type == 3) str = "GREEN";
	else if (type == 4) str = "BLUE";

	return str;
}

void component_Implement()
{
	CGameObject* object = CGameObject::create();
	object->insertComponent(new CImageRenderComponent);
	object->insertComponent(new CAbsolutePositionComponent);

	std::cout << object->getComponent<CRenderComponent>()->getComponentName_() << std::endl;
	std::cout << object->getComponent<CPositionComponent>()->getComponentName_() << std::endl;

	delete object;
}
/******************************************************************************************************/


// 아래 함수들은 C언어 전용 쓰레드 함수들입니다.

HANDLE hThread_1;
HANDLE hThread_2;
HANDLE hThread_3;
unsigned int* threadID_1 = nullptr;
unsigned int* threadID_2 = nullptr;
unsigned int* threadID_3 = nullptr;

void C_type_beginthreadex()
{
	printf("Creating second thread...\n");
#if(0)
	{
		// Create the second thread.
		hThread_1 = (HANDLE)_beginthreadex(NULL, 0, &SecondThreadFunc_1, NULL, 0, threadID_1);
		hThread_3 = (HANDLE)_beginthreadex(NULL, 0, &SecondThreadFunc_3, NULL, 0, threadID_3);

		// Wait until second thread terminates. If you comment out the line
		// below, Counter will not be correct because the thread has not
		// terminated, and Counter most likely has not been incremented to
		// 1000000 yet.
		WaitForSingleObjectEx(hThread_1, INFINITE, TRUE);
		WaitForSingleObjectEx(hThread_2, INFINITE, TRUE);
		WaitForSingleObjectEx(hThread_3, INFINITE, TRUE);
	}
#else
	{
		//window CE 5.0 ~ 6.0
		hThread_1 = (HANDLE)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Window_CE_SecondThreadFunc_1, NULL, 0, (LPDWORD)threadID_1);
		hThread_3 = (HANDLE)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Window_CE_SecondThreadFunc_3, NULL, 0, (LPDWORD)threadID_3);

		WaitForSingleObject(hThread_1, INFINITE);
		WaitForSingleObject(hThread_3, INFINITE);
	}
#endif
	CloseHandle(hThread_1);
	CloseHandle(hThread_2);
	CloseHandle(hThread_3);
	//printf("Counter should be 1000000; it is-> %d\n", Counter);
	// Destroy the thread object.
	
}

void C_type_beginthread()
{
	int param = 0;
	int * pparam = &param;

	// Get display screen's text row and column information.
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOut, &csbi);

	// Launch CheckKey thread to check for terminating keystroke.
	_beginthread(CheckKey, 0, NULL);

	// Loop until CheckKey terminates program or 1000 threads created. 
	while (repeat && param < 1000)
	{
		// launch another character thread.
		_beginthread(Bounce, 0, (void *)pparam);

		// increment the thread parameter
		param++;

		// Wait one second between loops.
		Sleep(1000L);
	}

}

unsigned __stdcall SecondThreadFunc_1(void* pArguments)
{

	int Counter = 0;
	printf("In second thread...1\n");
	
	hThread_2 = (HANDLE)_beginthreadex(NULL, 0, &SecondThreadFunc_2, NULL, 0, threadID_2);

	while (Counter < 10)
	{
		Sleep(1000);
		Counter++;
		printf("In second thread...1\n");
	}

	printf("End of Thread 1 \n");
	_endthreadex(0);
	return 0;
}

unsigned __stdcall SecondThreadFunc_2(void* pArguments)
{
	int Counter = 0;

	printf("In second thread...2\n");

	while (Counter < 10)
	{
		Sleep(1000);
		Counter++;
		printf("In second thread...2\n");
	}
	printf("End of Thread 2 \n");
	return 0;
}

unsigned __stdcall SecondThreadFunc_3(void* pArguments)
{
	int Counter = 0;

	printf("In second thread...3\n");

	while (Counter < 10)
	{
		Sleep(1000);
		Counter++;
		printf("In second thread...3\n");
	}
	printf("End of Thread 3 \n");
	_endthreadex(0);
	return 0;
}

// CheckKey - Thread to wait for a keystroke, then clear repeat flag.
void CheckKey(void * ignored)
{
	_getch();
	repeat = 0;    // _endthread implied
}

// Bounce - Thread to create and and control a colored letter that moves
// around on the screen.
//
// Params: parg - the value to create the character from
void Bounce(void * parg)
{
	char       blankcell = 0x20;
	CHAR_INFO  ci;
	COORD      oldcoord, cellsize, origin;
	DWORD      result;
	SMALL_RECT region;

	cellsize.X = cellsize.Y = 1;
	origin.X = origin.Y = 0;

	// Generate location, letter and color attribute from thread argument.
	srand(_threadid);
	oldcoord.X = region.Left = region.Right =
		GetRandom(csbi.srWindow.Left, csbi.srWindow.Right - 1);
	oldcoord.Y = region.Top = region.Bottom =
		GetRandom(csbi.srWindow.Top, csbi.srWindow.Bottom - 1);
	ci.Char.AsciiChar = GetGlyph(*((int *)parg));
	ci.Attributes = GetRandom(1, 15);

	while (repeat)
	{
		// Pause between loops.
		Sleep(100L);

		// Blank out our old position on the screen, and draw new letter.
		WriteConsoleOutputCharacterA(hStdOut, &blankcell, 1, oldcoord, &result);
		WriteConsoleOutputA(hStdOut, &ci, cellsize, origin, &region);

		// Increment the coordinate for next placement of the block.
		oldcoord.X = region.Left;
		oldcoord.Y = region.Top;
		region.Left = region.Right += GetRandom(-1, 1);
		region.Top = region.Bottom += GetRandom(-1, 1);

		// Correct placement (and beep) if about to go off the screen.
		if (region.Left < csbi.srWindow.Left)
			region.Left = region.Right = csbi.srWindow.Left + 1;
		else if (region.Right >= csbi.srWindow.Right)
			region.Left = region.Right = csbi.srWindow.Right - 2;
		else if (region.Top < csbi.srWindow.Top)
			region.Top = region.Bottom = csbi.srWindow.Top + 1;
		else if (region.Bottom >= csbi.srWindow.Bottom)
			region.Top = region.Bottom = csbi.srWindow.Bottom - 2;

		// If not at a screen border, continue, otherwise beep.
		else
			continue;
		Beep((ci.Char.AsciiChar - 'A') * 100, 175);
	}
	// _endthread given to terminate
	_endthread();
}


unsigned int Window_CE_SecondThreadFunc_1(LPTHREAD_START_ROUTINE *)
{
	
	//window CE 5.0 ~ 6.0
	int Counter = 0;
	hThread_2 = (HANDLE)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Window_CE_SecondThreadFunc_2, NULL, 0, (LPDWORD)threadID_2);
	
	printf("In second thread..................1\n");
	while (Counter < 10)
	{
		Sleep(1000);
		Counter++;
		printf("In second thread...1\n");
	}
	//WaitForSingleObject(hThread_2, INFINITE);
	printf("End of Thread 1 \n");

	ExitThread(0);

	return 0;
}

unsigned int Window_CE_SecondThreadFunc_2(LPTHREAD_START_ROUTINE *)
{
	printf("In second thread..................2\n");
	int Counter = 0;
	while (Counter < 10)
	{
		Sleep(500);
		Counter++;
		printf("In second thread...2\n");
	}
	printf("End of Thread 2 \n");
	return 0;
}

unsigned int Window_CE_SecondThreadFunc_3(LPTHREAD_START_ROUTINE *)
{
	printf("In second thread..................3\n");
	int Counter = 0;
	while (Counter < 10)
	{
		Sleep(1000);
		Counter++;
		printf("In second thread...3\n");
	}
	printf("End of Thread 3 \n");
	ExitThread(0);
	return 0;
}
void variable_factor_function()
{

	int i = 0;
	using vectype = func_type<bool, double, double, double, double>;
	std::vector<vectype> datas;
	Args_class a;
	datas.push_back(make_class_member_function_sugar_type_2(&Args_class::total_function_control_1<double, double, double, double>, a));
	datas.push_back(make_class_member_function_sugar_type_2(&Args_class::total_function_control_2<double, double, double, double>, a));
	datas.push_back(make_class_member_function_sugar_type_2(&Args_class::total_function_control_3<double, double, double, double>, a));
	datas.push_back(make_class_member_function_sugar_type_2(&Args_class::total_function_control_4<double, double, double, double>, a));
	datas.push_back(make_function_obj_sugar_type_2(a));

	datas.push_back(
		make_function_obj_sugar_type_2([](double x, double y, double z, double d) mutable {
		cout << " make_function_obj_sugar_type_2 : " << x << y << z << endl;
		std::cout << "lamda-Call" << endl;
		return true;
	}
		)
	);

	while (1)
	{
		for (auto&& item : datas)
		{
			//Sleep(1);
			if ((i % 4) == 0)
			{
				(*item)(10.0, 20.0, 30.0, (double)i);
				i++;
			}
			else if ((i % 4) == 1)
			{
				(*item)(20.0, 40.0, 30.0, (double)i);
				i++;
			}
			else if ((i % 4) == 2)
			{
				(*item)(70.0, 20.0, 30.0, (double)i);
				i++;
			}
			else if ((i % 4) == 3)
			{
				(*item)(80.1, 20.2, 90.3, (double)i);
				i++;
			}

		}
		break;
	}
	
}

#define NUM_THREAD	50
unsigned WINAPI threadInc(void * arg);
unsigned WINAPI threadDes(void * arg);
long long num = 0;
unsigned int* threadID_threadInc = nullptr;
unsigned int* threadID_threadDes = nullptr;

void C_type_beginthread_in_Muti()
{
	HANDLE tHandles[NUM_THREAD];
	int i;

	printf("sizeof long long: %d \n", sizeof(long long));

#if(0)
	for (i = 0; i<NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

#else
	//Wince 5.0 ~ 6.0
		
	for (i = 0; i<NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&threadInc, NULL, 0, (LPDWORD)threadID_threadInc);
		else
			tHandles[i] = (HANDLE)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&threadDes, NULL, 0, (LPDWORD)threadID_threadDes);
	}
	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);
	for (i = 0; i<NUM_THREAD; i++)	CloseHandle(tHandles[i]);
	
#endif
	
}

void C_type_parameter_delivery_Multi_thread()
{
	PMYDATA pDataArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];

	// Create MAX_THREADS worker threads. 

	/*
	1.2.2._beginthread, _beginthreadex
	새로운 쓰레드를 만든다.CreateThread와 유사하지만 다음의 차이점이 있다.•쓰레드에 여러 인수를 전달할 수 있다.
	•특정한 C 런타임 라이브러리 변수를 초기화 한단다..이게 무슨 의미인지 좀 알아봐야 겠다.
	•CreateThread는 보안특성을 제어한다.이 함수로 일시 중단된 상태에서 쓰레드를 시작할 수 있단다.역시 무슨 의미인지 알아봐야 겠다.왜이리 말들이 헛갈리냐.
	_beginthread함수는 쓰레드를 생성하자 말자 핸들을 닫아 버리기 때문에, 거의 사용하지 않는다.
	*/

	std::cout << "생성생성,생성생성,생성생성,생성생성," << std::endl;

	for (int i = 0; i<MAX_THREADS; i++)
	{
		// 쓰레드 데이터를 위한 메모리 할당 
		pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(MYDATA));

		if (pDataArray[i] == NULL)
		{
			// 메모리 할당에 실패하면 종료한다. 
			ExitProcess(2);
		}

		// 각 쓰레드에 넘겨 줄 데이터의 값을 설정한다. 
		pDataArray[i]->val1 = i;
		pDataArray[i]->val2 = i + 100;

		// 쓰레드를 생성한다. 
		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes 
			0,                      // use default stack size   
			MyThreadFunction,       // thread function name 
			pDataArray[i],          // argument to thread function  
			0,                      // use default creation flags  
			&dwThreadIdArray[i]);   // returns the thread identifier  

									// 쓰레드가 성공적으로 생성되었는지를 확인한다. 
									// 생성에 실패하면 프로세스를 종료한다. 
		if (hThreadArray[i] == NULL)
		{
			ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	}

	// 쓰레드 종료를 기다린다. 
	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

	// 모든 쓰레드 핸들러를 닫고, 메모리를 해제한다. 
	for (int i = 0; i<MAX_THREADS; i++)
	{
		CloseHandle(hThreadArray[i]);
		if (pDataArray[i] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
			pDataArray[i] = NULL;
		}
	}
}

void C_type_parameter_beginethread_thread()
{
	DWORD cntOfThread = 0;
	DWORD dwThreadID[MAX_THREADS_];
	HANDLE hThread[MAX_THREADS_];

	DWORD Total = 0;
	DWORD Result = 0;

	std::cout << "C_type_parameter_beginethread_thread" << std::endl;

	while (1) {
		if (cntOfThread == MAX_THREADS_) {
			_tprintf(_T("MAXIMUM THREAD NUMBER : %d\n"), cntOfThread);
			break;
		}
		hThread[cntOfThread] =
			(HANDLE)_beginthreadex(       //HANDLE로의 형변환이 필요합니다.
				NULL, 0, UINT_ThreadProc,
				(LPVOID)cntOfThread,
				0,
				(unsigned *)&dwThreadID[cntOfThread]);
		cntOfThread++;
	}  //while문을 이용하여 10개의 스레드 생성

	Sleep(1000);
	// 스레드가 자신의 할일을 모두 수행하고, 리턴값을 남기고 사라짐
	// 리턴값을 이용하여 Total 계산

	for (DWORD i = 0; i<cntOfThread; i++) {
		GetExitCodeThread(hThread[i], &Result);
		Total += Result;
		CloseHandle(hThread[i]);
	}

	_tprintf(_T(" Total :: %d \n"), Total);      //스레드의 리턴값으로 얻은 Total
	_tprintf(_T("STotal :: %d \n"), STotal);   //스레드가 전역변수에 접근하여 계산된 값
	
}

void C_type_parameter_createthread_thread()
{
	DWORD cntOfThread = 0;
	DWORD dwThreadID[MAX_THREADS_];
	HANDLE hThread[MAX_THREADS_];

	DWORD Total = 0;
	DWORD Result = 0;

	std::cout << "C_type_parameter_createthread_thread" << std::endl;

	while (1) {
		if (cntOfThread == MAX_THREADS_) {
			_tprintf(_T("MAXIMUM THREAD NUMBER : %d\n"), cntOfThread);
			break;
		}
		hThread[cntOfThread] =
			CreateThread(
				NULL, 0, DWORD_ThreadProc,
				(LPVOID)cntOfThread,
				0,
				&dwThreadID[cntOfThread]);
		cntOfThread++;
	}  //while문을 이용하여 10개의 스레드 생성

	Sleep(1000);
	// 스레드가 자신의 할일을 모두 수행하고, 리턴값을 남기고 사라짐
	// 리턴값을 이용하여 Total 계산

	for (DWORD i = 0; i<cntOfThread; i++) {
		GetExitCodeThread(hThread[i], &Result);
		Total += Result;
		CloseHandle(hThread[i]);
	}

	_tprintf(_T(" Total :: %d \n"), Total);      //스레드의 리턴값으로 얻은 Total
	_tprintf(_T("STotal :: %d \n"), STotal);   //스레드가 전역변수에 접근하여 계산된 값
	
}

void CPP_type_parameter_delivery_thread()
{
	MyWrapper mw;
	char msg[] = "Hi there.";
	HANDLE threadHandle = mw.createThread(1, msg);
	WaitForSingleObject(threadHandle, INFINITE);
}

void Chrono_time_check()
{
	auto t = timer();
	auto count = 150000000;
	t.reset();
	for (int i = 0; i < count; ++i) {
		volatile int temp = 10;
		volatile int temp2 = (temp * count) % 3;
		volatile int temp3 = (temp2 * temp * temp2) % count;
		if (i == 25000000) {
			t.record("25000000 count: ");
			t.reset();
		}
		if (i == 75000000) {
			t.record("50000000 count: ");
			t.reset();
		}
	}
	t.record("75000000 count: ");

	std::cout << std::endl << "convert test : " << std::endl;
	std::cout << timer::cast<nano_t>(t.get()) << " nano" << std::endl;
	std::cout << timer::cast<micro_t>(t.get()) << " micro" << std::endl;
	std::cout << timer::cast<milli_t>(t.get()) << " milli" << std::endl;
	std::cout << timer::cast<sec_t>(t.get()) << " sec" << std::endl << std::endl;

	SHOW_TIME_CONVERT(1, hour_t, min_t)
	SHOW_TIME_CONVERT(1, min_t, sec_t)
	SHOW_TIME_CONVERT(1, sec_t, milli_t)
	SHOW_TIME_CONVERT(1, milli_t, micro_t)
	SHOW_TIME_CONVERT(1, micro_t, nano_t)


	/**********************************************************************/
	std::cout << "countdown:\n";
	for (int i = 10; i>0; --i) {
		std::cout << i << std::endl;
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
	}
	std::cout << "Lift off!\n";

	/************************************************************************/

	using namespace std::chrono_literals;
	std::cout << "Hello waiter" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000));
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	std::cout << "Waited " << elapsed.count() << " ms\n";

}

void C_type_parameter_delivery_thread()
{
	char msg[] = "Hi there.";
	PARAMS params;
	params.i = 1;
	params.msg = msg;

	HANDLE threadHandle = CreateThread(NULL, 0, myThread, &params, 0, NULL);
	WaitForSingleObject(threadHandle, INFINITE);
}

DWORD WINAPI myThread(void* parameter)
{
	PARAMS* params = (PARAMS*)parameter;
	printf("Received parameters: i = %d, msg = '%s'\n", params->i, params->msg);
	return 0;
}

unsigned WINAPI threadInc(void * arg)
{
	int i;
	for (i = 0; i<50000000; i++)
		num += 1;
	return 0;
}
unsigned WINAPI threadDes(void * arg)
{
	std::cout << num << std::endl;
	return 0;
}

// 쓰레드 함수 
DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	HANDLE hStdout;
	PMYDATA pDataArray;

	TCHAR msgBuf[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;

	// 표준출력을 제어하기 위한 콘솔을 만든다. 
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	// Cast the parameter to the correct data type. 
	// The pointer is known to be valid because  
	// it was checked for NULL before the thread was created. 
	pDataArray = (PMYDATA)lpParam;

	// Print the parameter values using thread-safe functions. 
	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"),
		pDataArray->val1, pDataArray->val2);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

	return 0;
}
void ErrorHandler(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code. 

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message. 

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	// Free error-handling buffer allocations. 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
DWORD WINAPI DWORD_ThreadProc(LPVOID lpParam) {
	STotal += (DWORD)lpParam; // 전역변수도 접근 가능
	return (DWORD)lpParam;      // 리턴값       전달 가능
}
UINT WINAPI UINT_ThreadProc(LPVOID lpParam) { //unsigned int 자료형
	STotal += (DWORD)lpParam; // 전역변수도 접근 가능
	return (DWORD)lpParam;      // 리턴값       전달 가능
}