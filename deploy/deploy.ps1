$PSScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
docker build -t deploy -f $PSScriptDir/Dockerfile $PSScriptDir/..
if ($LASTEXITCODE -ne 0) {
    Write-Error "Docker build failed"
    exit 1
}
docker run -v ${HOME}/.aws:/root/.aws -it deploy
