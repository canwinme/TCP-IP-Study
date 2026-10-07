# TCP/IP Study

Linux 환경에서 C 언어로 TCP 소켓 통신과 POSIX Thread(pthread)를 단계별로 실습한 코드 모음입니다.

## 학습 흐름

```text
기본 TCP 연결
    ↓
Echo 통신
    ↓
pthread 기본
    ↓
mutex 동기화
    ↓
멀티스레드 다중 채팅
```

## 폴더 구조

```text
TCP-IP-Study/
├─ README.md
├─ basic/
│  ├─ hello_server.c
│  └─ hello_client.c
├─ echo/
│  ├─ echo_server.c
│  └─ echo_client.c
├─ pthread/
│  ├─ thread1.c
│  └─ thread2.c
└─ multi-chat/
   ├─ multi_thread_multi_chatting_server.c
   └─ multi_thread_multi_chatting_client.c
```

## 1. Basic TCP

### hello_server.c
서버가 클라이언트 연결을 기다린 뒤 고정 메시지를 한 번 전송하는 기본 TCP 서버입니다.

주요 흐름:

```text
socket()
→ bind()
→ listen()
→ accept()
→ write()
→ close()
```

실행 예시:

```bash
gcc hello_server.c -o hello_server
./hello_server 9999
```

### hello_client.c
서버 IP와 포트를 받아 연결한 뒤 서버가 보낸 메시지를 읽는 기본 TCP 클라이언트입니다.

```bash
gcc hello_client.c -o hello_client
./hello_client 127.0.0.1 9999
```

## 2. Echo Server / Client

### echo_server.c
클라이언트가 보낸 데이터를 읽은 뒤 같은 내용을 그대로 다시 보내는 Echo 서버입니다.

### echo_client.c
사용자가 입력한 문자열을 서버로 전송하고, 서버에서 되돌아온 메시지를 출력합니다.

```text
Client 입력
   ↓
write()
   ↓
Server read()
   ↓
Server write()
   ↓
Client read()
```

## 3. pthread

### thread1.c
두 개의 스레드를 생성하고 `pthread_join()`으로 종료를 기다리는 기본 스레드 실습입니다.

주요 함수:

- `pthread_create()`
- `pthread_join()`

### thread2.c
여러 스레드가 하나의 공유 변수에 접근하는 상황에서 mutex를 이용해 임계영역을 보호하는 실습입니다.

주요 함수:

- `pthread_mutex_init()`
- `pthread_mutex_lock()`
- `pthread_mutex_unlock()`
- `pthread_mutex_destroy()`

## 4. Multi-thread Multi-chatting

### multi_thread_multi_chatting_server.c
여러 클라이언트의 접속을 받아 각 클라이언트를 별도 스레드로 처리하고, 수신한 메시지를 연결된 클라이언트 전체에 전달하는 구조입니다.

주요 기능:

- TCP 서버 소켓 생성
- 다중 클라이언트 접속
- 클라이언트별 `pthread_create()`
- 클라이언트 소켓 배열 관리
- mutex를 이용한 공유 데이터 보호
- 전체 클라이언트 메시지 broadcast

컴파일:

```bash
gcc multi_thread_multi_chatting_server.c -o chat_server -pthread
./chat_server 9999
```

### multi_thread_multi_chatting_client.c
송신 스레드와 수신 스레드를 분리해 메시지를 입력하면서 동시에 다른 클라이언트의 메시지를 받을 수 있도록 구현한 클라이언트입니다.

주요 기능:

- 닉네임 지정
- 송신/수신 스레드 분리
- `q` 또는 `Q` 입력 시 종료

컴파일:

```bash
gcc multi_thread_multi_chatting_client.c -o chat_client -pthread
./chat_client 127.0.0.1 9999 nickname
```

## 학습 내용

- IPv4 TCP 소켓 통신
- `sockaddr_in`
- `socket()`, `bind()`, `listen()`, `accept()`, `connect()`
- `read()`, `write()`
- Blocking I/O
- POSIX Thread
- Thread Join / Detach
- Critical Section
- Mutex
- Multi-client socket 관리
- 메시지 Broadcast

## 개발 환경

- Linux
- C
- GCC
- POSIX Socket API
- pthread
