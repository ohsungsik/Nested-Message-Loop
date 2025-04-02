# Modal Timer Nested Message Loop Demo

## 프로젝트 소개

이 프로젝트는 Windows 프로그래밍에서 흔히 놓칠 수 있는 메시지 박스(`MessageBox`)의 내부 메시지 루프와 타이머(`WM_TIMER`) 메시지의 상호작용을 보여주는 예제입니다. 특히 메시지 박스가 내부적으로 메시지 루프를 처리함으로써 발생할 수 있는 중첩된 메시지 루프(Nested Message Loop)에 의한 상태 변화 문제를 직관적으로 시연하기 위한 목적으로 개발되었습니다.

## 프로젝트 목표

- 모달(MessageBox) 대화상자의 내부 메시지 루프가 어떻게 작동하는지 시연
- WM_TIMER 메시지와 MessageBox 간의 상호작용 이해
- 메시지 박스가 표시되는 동안 타이머에 의해 변수 상태가 변경되는 시나리오를 통해 중첩된 메시지 루프(Nested Message Loop)의 개념을 명확히 전달

## 핵심 내용

- 단 한 번의 함수 호출(`MessageTimer()`)로 두 개의 메시지 박스("Hello", "World")가 순차적으로 표시되는 상황 재현
- WM_TIMER 메시지를 활용한 타이밍 기반 상태 변화 구현
- 주석을 통해 코드 흐름과 동작 원리 설명

## 기술 스택

- **언어**: C++17
- **API**: Win32 API (Windows 전용)
- **개발 환경**: Visual Studio

## 실행 방법

1. Visual Studio로 프로젝트를 열고 빌드하거나, 루트 디렉터리의 `build.bat` 스크립트를 실행하여 빌드합니다.
2. 빌드 결과물은 `\out\bin` 디렉터리에 생성됩니다.
3. 생성된 실행 파일을 실행하면, 자동으로 두 개의 메시지 박스가 순차적으로 표시됩니다.

## 시나리오 설명

- 창 생성 시 타이머(WM_TIMER)가 설정되고, `MessageTimer()` 함수가 호출됩니다.
- 첫 번째 메시지 박스("Hello")가 표시될 때 내부 메시지 루프가 동작하여 WM_TIMER 메시지를 처리합니다.
- 이로 인해 상태 변수(`gCount`)가 변경되어, 첫 번째 메시지 박스가 닫히자마자 두 번째 메시지 박스("World")가 자동으로 표시됩니다.

## 주요 코드

```cpp
case WM_CREATE:
{
	// 전역 변수 gCount 초기화
	gCount = 1;

	// 100ms 간격으로 WM_TIMER 설정
	SetTimer(hwnd, eTimerId::TimerMessageBox, 100, nullptr);

	// MessageTimer 호출하여 메시지 박스 표시
	ShowMessageBoxes();

	// 타이머로 인해 gCount가 변경되었다면 프로그램 종료
	if (gCount == 2)
		PostQuitMessage(0);
}
return 0;

// gCount 값에 따라 메시지 박스를 순차적으로 표시
void ShowMessageBoxes()
{
	if (gCount == 1) 
		MessageBoxW(nullptr, L"Hello", L"", MB_OK);

	// 메시지 박스 표시 중 WM_TIMER가 처리되어 gCount가 2가 되면
	// 두 번째 메시지 박스 표시
	if (gCount == 2) 
		MessageBoxW(nullptr, L"World", L"", MB_OK);
}
```

## 기대 효과

이 프로젝트를 통해 개발자는 모달 다이얼로그 및 타이머 메시지 처리 시 발생 가능한 중첩된 메시지 루프 문제를 이해하고, 이를 예방하거나 효과적으로 관리하는 방법에 대해 생각해볼 수 있습니다.

## 라이센스

이 프로젝트는 학습 목적으로 제공되며 자유롭게 사용 및 수정 가능합니다.

