#include "http-driver.hh"

namespace NetworkDriver
{
  HttpDriver::HttpDriver()
  {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
      std::cerr << "Could not create socket..." << std::endl;
  }

  HttpDriver::~HttpDriver()
  {
    if (fd_ != -1)
      close(fd_);
  }

  int HttpDriver::sendRequest(const UrlParser& urlParser, const TrackerInfo& trackerInfo)
  {
    std::string request = "GET " + urlParser.getBody() + " HTTP/1.1\r\n";
    request += "Host: " + urlParser.getHost() + "\r\n";
    request += "Accept: */*\r\n";
    request += "Content-Length: 0\r\n";
    request += "\r\n";

    if (connect(fd_, (struct sockaddr*)&trackerInfo.getServerAddress(),
                sizeof (trackerInfo.getServerAddress())) < 0)
    {
      std::cerr << "Could not connect to url " << urlParser.getBaseUrl() << std::endl;
      return -1;
    }

    if (write(fd_, request.c_str(), strlen(request.c_str())) < 0)
    {
      std::cerr << "Could not write request to url " << urlParser.getBaseUrl() << std::endl;
      return -1;
    }

    return 1;
  }

  std::string HttpDriver::readResult()
  {
    std::string res = "";
    char *buffer = (char*)calloc(sizeof (char), READ_BUF_SIZE);
    int recvN = recv(fd_, buffer, READ_BUF_SIZE - 1, 0);
    while (recvN > 0)
    {
      buffer[READ_BUF_SIZE - 1] = '\0';
      if (recvN > 0)
      {
        for (int i = 0; i < recvN; i++)
          res += buffer[i];

        if (buffer[recvN - 1] == '\n' && buffer[recvN - 2] == '\r')
          break;
      }
      bzero(buffer, READ_BUF_SIZE);
      recvN = recv(fd_, buffer, READ_BUF_SIZE - 1, 0);
    }

    free(buffer);

    //formatResult(res);

    return res;
  }
}
