# Action-Game
UE 5.0.3을 활용한 Action Game 개발  
1인개발  

실행 동영상: https://www.youtube.com/watch?v=W23CpZrx-DE  

1) 개발 기간: 2023.03.24 ~ 2023.04.24  

2) 개발 환경: 언리얼 5.0.3, C++  

3) 목표: 언리얼 엔진5를 활용한 1인 RPG 게임 개발  

4) 주요 개발 내용:



- AnimInstance/Montage/StateMachine/IK Retargeting 을 활용한 플레이어 애니메이션
  - (Player/ 폴더 내 클래스들)
  - PlayerCharcter, Warrior : 처음 개발한 캐릭터
  - AGPlayer, Valkyrie : 7월 말 부터 개발한 캐릭터  

- Valkyrie 스킬 3종류, Camera Switch
  - Player/Valkyrie.cpp에서 확인 가능
  - Skill/Valkyrie/ 내 클래스에서 SkillActor 확인 가능

- Valkyrie Targeting 기능
  - Player/TargetingComponent.cpp, TargetingComponent.h 에서 확인 가능
  

- 일반 몬스터 2종류, 보스 몬스터 (스킬 3종류), Behavior Tree를 활용한 몬스터 AI
  - (Monster / AI / 폴더 내 클래스들)  

- UMG를 활용한 Widget (UI, Inventory, StatMenu)
  - (Widget/ 폴더 내 클래스들)
  - (Widget/Button/ 폴더 내에서 Button 관련 클래스 확인 가능)
  - (Widget/Stat/ 폴더 내에서 StatMenu 관련 클래스 확인 가능)
  - (Widget/WidgetController/ 폴더 내에서 WidgetController 관련 클래스 확인 가능)

- DataTable을 활용한 Player, Monster, Item Data 로드
  - (AGGameInstance.cpp, AGGameInstance.h)  

