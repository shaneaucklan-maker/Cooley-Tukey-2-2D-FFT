C++(MFC) 환경에서 외부 라이브러리 없이 직접 구현한 2D FFT 엔진과 시각화 모듈의 실행 방법입니다.



<프로젝트 생성>

•	Visual Studio에서 다중 문서(MDI) 기반 MFC 애플리케이션을 생성합니다. 프로젝트 이름은 makingfft로 설정합니다.

<bmp파일 배치>
•	생성된 MFC 앱의 루트 경로(프로젝트 폴더)에 butterfly.bmp 파일을 위치시킵니다.

<헤더 및 뷰 코드 작성>
•	프로젝트에 FFTEngine.h 파일을 추가하고 첨부된 FFT 엔진 코드를 기입합니다.
•	makingfftView.h와 makingfftView.cpp 파일에 첨부된 뷰 관련 코드를 각각 작성합니다.

<메뉴 리소스 설정>
•	리소스 뷰(Resource View)의 Menu 항목에서 IDR_makingfftTYPE을 엽니다.
•	상단 메뉴에 FFT를 추가하고, 하위 메뉴에 2D FFT 실행을 입력합니다.
•	생성된 메뉴의 ID를 ID_PROCESS_FFT로 변경합니다.

<빌드 및 실행>
•	F5 키를 눌러 실행한 뒤, 뷰 창에 출력된 원본 butterfly.bmp 이미지와 2D FFT 처리된 스펙트럼 결과를 확인합니다.
