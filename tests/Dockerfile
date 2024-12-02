FROM amazonlinux:2023
RUN yum install -y python3 python3-pip
RUN pip3 install requests

RUN yum install -y gcc-11.4.1-2.amzn2023.0.2
WORKDIR /build
COPY src src/
COPY tests/function.c src/
RUN mkdir -p target
RUN gcc -o target/bootstrap src/runtime.c src/function.c

WORKDIR /test
COPY tests/test_runtime.py .
CMD ["python3", "test_runtime.py", "/build/target/bootstrap"]
