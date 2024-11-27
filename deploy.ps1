docker build -t deploy -f Dockerfile.deploy .
docker run -v ${HOME}/.aws:/root/.aws -it deploy
