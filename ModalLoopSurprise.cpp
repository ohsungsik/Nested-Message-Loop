// \file ModalLoopSurprise.cpp
// \last_updated 2025-04-01
// \author oss <oss@yuwoll.com>
// \copyright (C) 2025. Yuwoll Co. All rights reserved.


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// 타이머 식별자(enum): TimerMessageBox 값은 1로 정의됨
enum eTimerId
{
	TimerMessageBox = 1,
};

// 전역 변수
// 메시지 박스 출력 순서를 결정하기 위한 카운터
int gCount;

// 윈도우 프로시저 함수 선언
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ShowMessageBoxes 함수 선언: 메시지 박스를 조건에 따라 호출
void			 ShowMessageBoxes();

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int nCmdShow)
{
	// Register the window class.
	constexpr const wchar_t className[] = L"Nested Message Loop";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;

	RegisterClass(&wc);

	// Create the window.
	const HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		className,                     // Window class
		L"Nested Message Loop",			// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		nullptr,		// Parent window    
		nullptr,		// Menu
		hInstance,		// Instance handle
		nullptr			// Additional application data
	);

	if (hwnd == nullptr)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			// 준비.
			// 전역 변수 gCount 초기화: 1로 시작
			gCount = 1;

			// 타이머 설정: 100ms 간격으로 WM_TIMER 메시지를 발생시킴
			SetTimer(hwnd, eTimerId::TimerMessageBox, 100, nullptr);

			// ShowMessageBoxes 함수 호출하여 메시지 박스 표시
			ShowMessageBoxes();

			/// 만약 타이머 이벤트로 인해 gCount가 2로 변경되었다면,
			/// 모든 작업이 완료되었으므로 메시지 루프를 종료하기 위해 WM_QUIT 메시지 전송
			if (gCount == 2)
				PostQuitMessage(0);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			const HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>((COLOR_WINDOW + 1)));

			EndPaint(hwnd, &ps);
		}
		return 0;

	case WM_TIMER:
		{
			if (wParam == eTimerId::TimerMessageBox)
			{
				// gCount 값이 1인 경우에만 2로 증가시킴
				if (gCount == 1)
					gCount++;

				// 타이머 중복 실행을 방지하기 위해 타이머 종료
				KillTimer(hwnd, eTimerId::TimerMessageBox);
			}
			return 0;
		}

	// 그 외의 메시지는 기본 윈도우 프로시저로 전달
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

// gCount 값에 따라 모달 메시지 박스를 순차적으로 표시
void ShowMessageBoxes()
{
	// gCount가 1이면 "Hello" 메시지 박스 표시
	if (gCount == 1) MessageBoxW(nullptr, L"Hello", L"", MB_OK);

	/// 모달 메시지 박스가 표시되는 동안 내부 메시지 루프에서 WM_TIMER 메시지가
	/// 처리되어 gCount가 2로 변경된다면, "World" 메시지 박스 표시
	if (gCount == 2) MessageBoxW(nullptr, L"World", L"", MB_OK);
}
