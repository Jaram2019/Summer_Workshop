# Summer_Workshop
2019 하계 워크샵 (thread를 이용한 proxy 서버 구축)

주제 : Thread를 이용한 proxy 서버 구축
- 에코 프록시 서버 구축

- client, proxy server, main server로 구성

1) gcc mainserver.c -o mainserver -lpthread
2) gcc proxy.c -o proxy -lpthread
3) gcc client.c -o client
4) ./mainserver
5) ./proxy 127.0.0.1 5010 5000
6) ./client
7) 클라이언트 터미널에서 프록시 서버 ip, port number인 127.0.0.1, 5000 입력
8) 메시지 입력하면 프록시를 거쳐 메인서버에 도착 후 다시 프록시를 거쳐 클라이언트로 되돌아옴
9) 각 지점을 거칠 때 마다 기존 메시지에 경로를 추가해줌
