docker build -t test -f Dockerfile.test .                                             
#docker run -v ${PWD}:/out -t test cp /build/target/bootstrap /out
docker run -it test