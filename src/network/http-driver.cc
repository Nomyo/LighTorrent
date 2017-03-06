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

  std::string HttpDriver::getResult()
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

    std::cout << "RESULT = " << res << std::endl;

    formatResult(res);

    return resultBody_;
  }

  void HttpDriver::formatResult(std::string& result)
  {
    //bool isChunked = false;

    std::string delimiter = "\r\n";
    size_t pos = 0;
    std::string token;
    while ((pos = result.find(delimiter)) != std::string::npos)
    {
      token = result.substr(0, pos);
      result.erase(0, pos + delimiter.length());
      resultHeader_ += token + '\n';

      //if (token == "Transfer-Encoding: chunked")
        //isChunked = true;

      if (token == "")
        break;
    }

    //if (isChunked)
    //{
    //  deleteChunkInfo(result);
    //  while ((pos = result.find(delimiter)) != std::string::npos)
    //  {
    //    token = result.substr(0, pos);
    //    result.erase(0, pos + delimiter.length());
    //    resultBody_ += token + '\n';
    //  }
    //}

    resultBody_ += result;
  }

  //void HttpDriver::deleteChunkInfo(std::string& s)
  //{
  //  int val = 2;
  //  int start_pos = 0;
  //  int next_pos = 0;
  //  while (val != 0)
  //  {
  //    int endline = s.find("\r\n", start_pos);
  //    std::string first_chunk = s.substr(start_pos, endline - start_pos);
  //    std::stringstream ss;
  //    ss << first_chunk.c_str();
  //    ss >> std::hex >> val;

  //    next_pos = start_pos + val + 2 + first_chunk.length();

  //    next_pos += 2;

  //    s.erase(start_pos, first_chunk.length() + 2);
  //    next_pos -= (first_chunk.length() + 2);

  //    start_pos = next_pos;
  //  }
  //}

}
