$PSScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
docker build -t test -f $PSScriptDir/Dockerfile $PSScriptDir/.. 
if ($LASTEXITCODE -ne 0) {
	Write-Error "Docker build failed"
	exit 1
}
docker run -it test