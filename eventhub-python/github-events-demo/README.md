# Exploring GitHub events with Azure Data Explorer

----

This project is based on Daniel Dubovski's [post](https://medium.com/microsoftazure/exploring-github-events-with-azure-data-explorer-69f28eb705b9) and python script for exploring GitHub events in ADX using EventHub.

The project demonstrates using Docker image which running the script in Azure container for maintaining an events live stream.  

## Setting up ADX and EventHub 

Follow the post  for setting up ADX, EventHub and integrate them:
[Exploring GitHub events with Azure Data Explorer](https://medium.com/microsoftazure/exploring-github-events-with-azure-data-explorer-69f28eb705b9)


## Using the existing Docker image

A compiled Docker image is available in Docker Hub under the name: **ariely15/githubeventskusto:v1.0.0**


## Building a Costume Docker file

1. Install [Docker](https://www.docker.com/get-started)
2. Create Docker Hub account.
3. Create costume docker file.
4. Build: 
`docker build --tag <account name>/<image name>:<version (example: v1.0.0)> .`

5. Test locally, run: 
`docker run -p 8080:80 -it <account name>/<image name>:v1.0.0`
6. Push image to Docker hub:
 `docker push <account name>/<image name>:v1.0.0`


## Configuration 

The Docker image updates the script from this GitHub repository each time its starts. As the script uses secrets for connecting to GitHub API and the EventHub instances, a configuration file must be added as a volume to the Azure container. 

The config file path should be:
`/home/config/EventHubsConfig.json`


## Deploying Docker image to an Azure container

Azure container can mount Docker image from Docker Hub, for more details about creating Azure container and adding configuration file as a volume please see: 
[Mount an Azure file share in Azure Container Instances](https://docs.microsoft.com/en-us/azure/container-instances/container-instances-volume-azure-files)


## Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.microsoft.com.

When you submit a pull request, a CLA-bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., label, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.
