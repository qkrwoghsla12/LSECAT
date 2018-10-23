# LSECAT
Ethercat Servo LSmecapion

ZYBO-Z7-20, Beagle Bone Black 에서 LS mecapion 사의 L7N 이더캣 서보를 제어하는 예제

## makefile 수정

|name| definition |
|--|--|
|XENOMAI_PATH | xenomai 경로|
|INC_ECAT     | ethercat 헤더 경로|
|LIB_DIR      | ethercat 라이브러리 경로|


## 실험 파라미터 변경

main.h

|name|definition |
|--|--|
|ECATCTRL_TASK_PRIORITY	| L7N 이더캣 서보를 제어하는 TASK 의 우선순위|
|CPUSPIN_TASK_PRIORITY	  | xenomai stress 를 주는 TASK 의 우선순위|
|ECATCTRL_TASK_PERIOD    | ECATCTRL_TASK 와 CPUSPIN_TASK 의 주기|
|SEC_DURATION            | 테스트를 수행하는 시간(s) |
|CPUSPIN                 | define 되어 있을 경우 ECATCTRL_TASK 외에 xenomai stress TASK 추가로 생성 define 되어있지 않을 경우 ECATCTRL_TASK 만 생성|
|PROCESS                 | define 되어 있을 경우 ECATCTRL_TASK 에 연산과정추가 |
|ECATCTRL_TASK | 이더캣 서보를 제어하는 TASK|
|CPUSPIN_TASK  | 시스템에 부하를 주기 위해 만든 더미 TASK|


## 실행 방법

bin 디렉토리에 있는 실행 아규먼트로 파일네임넣으면서 
예 

```
# ./bin/ecat-embedded StressTime(us) filename
```

|name| definition |
|--|--|
|StressTime(us) | CPUSPIN 이 define 되어 있을 경우 지정한 시간(us) 만큼 cpu를 소모함
|filename | ECATCTRL_TASK의 데이터가 저장될 파일명

아래 명령으로 TASK 들의 cpu 사용률을 관찰할 수 있다.

```
    # watch -n 1 "cat /proc/xenomai/stat" 
``` 

## 실행 결과
    프로그램 실행 때 지정한 이름의 파일로 ECATCTRL_TASK 의 데이터가 출력되고
    spintask_period.csv 파일 로 CPUSPIN_TASK 의 데이터가 출력된다.
    period, execution, jitter, collectTime, processTime, TranslateTime 순으로 데이터가 생성된다.
    bbb_stress250us.csv 는 사용자가 지정한 이름으로 ECATCTRL_TASK 의 결과이다.
    spintask_period.csv 는 CPUSPIN_TASK 의 결과이다.
    
 
    
    
    
