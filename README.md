# 네트워크 솔루션 부트캠프 과제
## 1. 비트 연산자를 이용한 계산기
## 2. 큰 수 연산을 할 수 있는 계산기
- 덧셈, 뺄셈만 가능
## 3. 파일을 활용한 RDBMS 모사하는 주소록
### 데이터는 USERDATA 구조체에서 관리한다
- 데이터 속성은 name, age, address, phone
- 테스트 데이터는 100만 건이었고 phone은 중복 없는 고유한 문자열
- 쿼리문은 and 연산자만 지원하며 예시는 다음과 같다
  - `insert name='김김김' and address='서울특별시' and phone = '000-0000-0000' age=100`
  - `select name='김김김' and address='서울특별시' and age=52`
  - `update set name='김김김' and address='부산광역시' where name='이이이'`
  - `delete name='김김김' and address = '서울특별시'`
- 모든 명령어는 띄어쓰기 단위로 파싱한다
  - 제대로 된 파싱을 위해 소문자 변환을 진행하며 `=` 앞뒤로 공백을 전부 제거한다
- 문자열에 홑따옴표를 붙여야 문자열 그대로 파싱이 가능하다
### 성능
첫 데이터 조회 시간   
![first](https://github.com/user-attachments/assets/b88716a7-f6c5-453b-b0ec-1cf63cacb476)

끝 데이터(100만 번째) 조회 시간    
![last](https://github.com/user-attachments/assets/3ce2d345-d56f-4ced-aa22-d0569ab5a388)

### 개선점
- 인덱스 사용
- 자료구조에 트리를 적용
  - 전반적인 구조 개선이 선행
