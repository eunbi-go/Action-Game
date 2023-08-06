# Action-Game
UE 5.0.3을 활용한 Action Game 개발  
1인개발  

실행 동영상: https://www.youtube.com/watch?v=W23CpZrx-DE  

1) 개발 기간: 2023.03.24 ~ 2023.04.24  

2) 개발 환경: 언리얼 5.0.3, C++  

3) 목표: 언리얼 엔진5를 활용한 1인 RPG 게임 개발  

4) 주요 개발 내용:  
Niagara System 과 Material 을 활용해 Ribbon Trail
(Basic / WeaponActor.cpp, WeaponActor.h)
AnimInstance/Montage/StateMachine/IK Retargeting 을 활용한 플레이어 애니메이션
(Player/ 폴더 내 클래스들)
플레이어 스킬 3종류
(Skill/ 폴더 내 클래스들)  
일반 몬스터 2종류, 보스 몬스터 (스킬 3종류), Behavior Tree를 활용한 몬스터 AI
(Monster / AI / 폴더 내 클래스들)  
UMG를 활용한 Widget (UI, Inventory)
(Widget/ 폴더 내 클래스들)
DataTable을 활용한 Player, Monster, Item Data 로드
(AGGameInstance.cpp, AGGameInstance.h)
레벨 전환 시 파일 입출력을 활용한 데이터 유지  
