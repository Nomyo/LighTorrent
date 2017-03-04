#include <string.h>
#include "tracker-connector.hh"

#define READ_BUF_SIZE 6000

namespace TrackerConnector
{
  TrackerConnector::TrackerConnector()
  {
    opened_ = false;
  }

  TrackerConnector::~TrackerConnector()
  {
    if (opened_)
      close(fd_);
  }

  int TrackerConnector::sendRequest(std::string url)
  {
    parseUrl(url);

    if (!createSocket())
      return -1;

    if (!resolveHost(host_))
      return -1;

    if (!craftRequest(url))
      return -1;

    return readResult();
  }

  std::string TrackerConnector::getResult()
  {
    std::cout << result_ << std::endl;

    bool isChunked = false;

    std::string delimiter = "\r\n";
    size_t pos = 0;
    std::string token;
    while ((pos = result_.find(delimiter)) != std::string::npos)
    {
      token = result_.substr(0, pos);
      std::cout << "lineh: \"" << token << "\"" << std::endl;
      result_.erase(0, pos + delimiter.length());
      result_header_ += token + '\n';

      if (token == "Transfer-Encoding: chunked")
        isChunked = true;

      if (token == "")
        break;
    }

    std::cout << "chunked = " << isChunked << " -- end of header\n";

    if (isChunked)
    {
      while ((pos = result_.find(delimiter)) != std::string::npos)
      {
        token = result_.substr(0, pos);
        std::cout << "lineb: \"" << token << "\"" << std::endl;
        result_.erase(0, pos + delimiter.length());
        result_body_ += token + '\n';
      }
    }

    result_body_ += result_;

    std::cout << "-- end of body\n";

    std::cout << "rh: " << result_header_ << std::endl;
    std::cout << "rb: " << result_body_ << std::endl;

    // result_ is now empty;
    return "";
    return result_;
  }

  int TrackerConnector::createSocket()
  {
    if (opened_)
    {
      std::cerr << "cannot create socket: already active" << std::endl;
      return -1;
    }

    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
    {
      std::cerr << "could not create socket" << std::endl;
      return -1;
    }

    opened_ = true;

    return 1;
  }

  int TrackerConnector::resolveHost(std::string host)
  {
    server_ = gethostbyname(host.c_str());

    if (server_ == nullptr)
    {
      std::cerr << "Could not resolve host " << host << std::endl;
      return -1;
    }

    bzero((char *) &servAddr_, sizeof (servAddr_));

    servAddr_.sin_family = AF_INET;

    bcopy((char *)server_->h_addr,
          (char *)&servAddr_.sin_addr.s_addr,
          server_->h_length);

    servAddr_.sin_port = htons(port_);

    host_ = host;

    return 1;
  }

  int TrackerConnector::craftRequest(std::string url)
  {
    std::string request = "GET " + request_ + " HTTP/1.1\r\n";
    request += "Host: " + host_ + "\r\n\r\n";

    if (connect(fd_, (struct sockaddr*)&servAddr_, sizeof (servAddr_)) < 0)
    {
      std::cerr << "Could not connect to url " << url << std::endl;
      return -1;
    }

    if (write(fd_, request.c_str(), strlen(request.c_str())) < 0)
    {
      std::cerr << "Could not write request to url " << url << std::endl;
      return -1;
    }

    return 1;
  }

  int TrackerConnector::readResult()
  {
    result_ = "";
    std::string res = "";
    char *buffer = (char*)calloc(sizeof (char), READ_BUF_SIZE);
    int recvN = recv(fd_, buffer, READ_BUF_SIZE - 1, 0);
    while (recvN > 0)
    {
      buffer[READ_BUF_SIZE - 1] = '\0';
      if (recvN > 0)
      {
        res += buffer;
        if (buffer[recvN - 1] == '\n' && buffer[recvN - 2] == '\r')
          break;
      }
      bzero(buffer, READ_BUF_SIZE);
      recvN = recv(fd_, buffer, READ_BUF_SIZE - 1, 0);
    }

    free(buffer);

    result_ = res;

    return 1;
  }

  void TrackerConnector::parseUrl(std::string url)
  {
    int counterStart = 0;
    bool hasHttp = false;
    if (url.substr(0, 7) == "http://")
    {
      counterStart = 7;
      hasHttp = true;
    }
    while (url[counterStart] != '/' && url[counterStart] != ':')
      counterStart++;

    host_ = url.substr(hasHttp ? 7 : 0, counterStart - (hasHttp ? 7 : 0));

    if (url[counterStart] == ':')
    {
      int left = ++counterStart;
      while (url[counterStart] != '/')
        counterStart++;
      std::string num = url.substr(left, counterStart - left);
      port_ = atoi(num.c_str());
    }
    else
      port_ = 80;

    request_ = url.substr(counterStart, url.length() - counterStart);

    std::cout << "host: " << host_ << std::endl;
    std::cout << "port: " << port_ << std::endl;
    std::cout << "request: " << request_ << std::endl;
  }
}
