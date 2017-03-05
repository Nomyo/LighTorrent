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
    urlParser_.parseUrl(url);

    if (!createSocket())
      return -1;

    if (!resolveHost(urlParser_.getHost()))
      return -1;

    if (!craftRequest(url))
      return -1;

    return readResult();
  }

  std::string TrackerConnector::getResult()
  {
    return resultHeader_ + resultBody_;
  }

  std::string TrackerConnector::getResultHeader()
  {
    return resultHeader_;
  }

  std::string TrackerConnector::getResultBody()
  {
    return resultBody_;
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

    servAddr_.sin_port = htons(urlParser_.getPort());

    return 1;
  }

  int TrackerConnector::craftRequest(std::string url)
  {
    std::string request = "GET " + urlParser_.getBody() + " HTTP/1.1\r\n";
    request += "Host: " + urlParser_.getHost() + "\r\n";
    request += "Accept: */*\r\n";
    request += "Content-Length: 0\r\n";
    request += "\r\n";

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

    std::cout << "Tracker GET request: " << request << std::endl;

    return 1;
  }

  int TrackerConnector::readResult()
  {
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

    formatResult(res);

    return 1;
  }

  void TrackerConnector::formatResult(std::string& result)
  {
    bool isChunked = false;

    std::string delimiter = "\r\n";
    size_t pos = 0;
    std::string token;
    while ((pos = result.find(delimiter)) != std::string::npos)
    {
      token = result.substr(0, pos);
      result.erase(0, pos + delimiter.length());
      resultHeader_ += token + '\n';

      if (token == "Transfer-Encoding: chunked")
        isChunked = true;

      if (token == "")
        break;
    }

    if (isChunked)
    {
      deleteChunkInfo(result);
      while ((pos = result.find(delimiter)) != std::string::npos)
      {
        token = result.substr(0, pos);
        result.erase(0, pos + delimiter.length());
        resultBody_ += token + '\n';
      }
    }

    resultBody_ += result;
  }

  void TrackerConnector::deleteChunkInfo(std::string& s)
  {
    int val = 2;
    int start_pos = 0;
    int next_pos = 0;
    while (val != 0)
    {
      int endline = s.find("\r\n", start_pos);
      std::string first_chunk = s.substr(start_pos, endline - start_pos);
      std::stringstream ss;
      ss << first_chunk.c_str();
      ss >> std::hex >> val;

      next_pos = start_pos + val + 2 + first_chunk.length();

      next_pos += 2;

      s.erase(start_pos, first_chunk.length() + 2);
      next_pos -= (first_chunk.length() + 2);

      start_pos = next_pos;
    }
  }
}
