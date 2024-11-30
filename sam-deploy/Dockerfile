FROM public.ecr.aws/sam/build-provided.al2023
WORKDIR /build
RUN mkdir -p target
COPY src src/
RUN gcc -O3 -DRELEASE -o target/bootstrap src/runtime.c src/function.c
RUN strip -s target/bootstrap
COPY sam-deploy/*.??ml ./
CMD ["sam", "deploy"]
