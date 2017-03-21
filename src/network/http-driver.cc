#include "http-driver.hh"

namespace Network
{
  using Peer = Network::Peer;

  HttpDriver::HttpDriver()
  {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
      std::cerr << "Could not create http socket..." << std::endl;
  }

  HttpDriver::~HttpDriver()
  {
    if (fd_ != -1)
      close(fd_);
  }

  std::vector<Peer> HttpDriver::announce(const UrlParser& urlParser, const TrackerInfo& trackerInfo)
  {
    using namespace BEncode;
    std::vector<Peer> peers;

    if (sendRequest(urlParser, trackerInfo) != -1)
    {
      BEncodeDriver driver;
      auto result_node = getType<BType_ptr, BDico>(driver.bDecode(resultBody_));
      std::cout << result_node << std::endl;
      std::string peersBinary = getDecode<BType_ptr, BString, std::string>(result_node.get("peers"));
      std::cout << peersBinary << std::endl;

      peers = buildPeers((const uint8_t *)peersBinary.c_str(), ((peersBinary.length() - 1) / 6));
    }

    return peers;
  }

  int HttpDriver::sendRequest(const UrlParser& urlParser, const TrackerInfo& trackerInfo)
  {
    std::string request = "GET " + urlParser.getBody() + " HTTP/1.1\r\n";
    request += "Host: " + urlParser.getHost() + "\r\n";
    request += "Accept: */*\r\n";
    request += "Content-Length: 0\r\n";
    request += "\r\n";

    std::cout << "Connecting to tracker..." << std::endl;
    if (connect(fd_, (struct sockaddr*)&trackerInfo.getServerAddress(),
                sizeof(trackerInfo.getServerAddress())) < 0)
    {
      std::cerr << "Could not connect to url " << urlParser.getBaseUrl() << std::endl;
      return -1;
    }

    std::cout << "Announcing to tracker..." << std::endl;
    if (write(fd_, request.c_str(), strlen(request.c_str())) < 0)
    {
      std::cerr << "Could not write request to url " << urlParser.getBaseUrl() << std::endl;
      return -1;
    }

    buildResult();

    return 1;
  }

  std::string HttpDriver::getResult()
  {
    return resultBody_;
  }

  void HttpDriver::buildResult()
  {
    std::string res = "";
    char *buffer = (char*)calloc(sizeof(char), READ_BUF_SIZE);
    int recvN = recv(fd_, buffer, READ_BUF_SIZE - 1, 0);

    buffer[READ_BUF_SIZE - 1] = '\0';
    if (recvN > 0)
    {
      for (int i = 0; i < recvN; i++)
        res += buffer[i];
    }
    bzero(buffer, READ_BUF_SIZE);

    free(buffer);

    formatResult(res);
  }

  void HttpDriver::formatResult(std::string& result)
  {
    std::string delimiter = "\r\n";
    size_t pos = 0;
    std::string token;
    while ((pos = result.find(delimiter)) != std::string::npos)
    {
      token = result.substr(0, pos);
      result.erase(0, pos + delimiter.length());
      resultHeader_ += token + '\n';

      if (token == "")
        break;
    }

    resultBody_ += result;
  }
}
