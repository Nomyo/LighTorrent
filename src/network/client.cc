#include "client.hh"

namespace Network
{

  Client::Client()
  { }

  Client::Client(Torrent torrent)
    : torrent_(torrent)
  { }

  Client::~Client()
  { }

  void Client::download(const std::string& filename)
  {
    BEncodeDriver driver;
    auto decodedFile = driver.bDecodeFile(filename);

    if (!decodedFile)
    {
      std::cerr << filename << " is not a valid torrent file" << std::endl;
      return;
    }

    torrent_ = (getType<BType_ptr, BDico>(decodedFile));

    Core::URLUtils url;
    std::string urlGenerated = url.generateURL(torrent_);
    UrlParser::UrlParser up(urlGenerated);
    up.dump();
    std::cout << std::endl;

    TrackerDriver trackDriver(&torrent_);
    trackDriver.createConnectors();
    trackDriver.announces();

    TrackerConnector::TrackerConnector tc(&torrent_);
    peers_ = tc.announce(urlGenerated);
    dumpPeers();
    connectToPeers();
  }

  void Client::getPeersFromBinary(const std::string& binaryPeers)
  {
    const unsigned char *str = (const unsigned char *) binaryPeers.c_str();
    int j = 0;
    in_port_t port = 0;
    std::string ip;

    for (unsigned i = 0; i < binaryPeers.size(); ++i, ++j)
    {
      unsigned int b = str[i];

      if (j == 4)
	port = 256 * str[i];
      else if (j == 5)
      {
	port += str[i];
	peers_.push_back(Peer(ip, port));
	port = 0;
	j = -1;
      }
      else
      {
	if (i % 6)
	  ip += ".";
	else
	  ip = "";
	ip += std::to_string(b);
      }
    }
  }

  void Client::connectToPeers()
  {
    for (auto peer : peers_)
    {
      int valopt;
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      fd_set myset;
      socklen_t lon;

      fcntl(sockfd, F_SETFL, O_NONBLOCK);

      struct sockaddr_in remoteaddr;
      remoteaddr.sin_family = AF_INET;
      remoteaddr.sin_addr.s_addr = inet_addr(peer.getIp().c_str());
      remoteaddr.sin_port = htons(peer.getPort());

      struct timeval tv;

      if (connect(sockfd, (struct sockaddr *)&remoteaddr, sizeof(remoteaddr)) < 0)
      {
	if (errno == EINPROGRESS)
	{
	  tv.tv_sec = 1;
	  tv.tv_usec = 0;

	  FD_ZERO(&myset);
	  FD_SET(sockfd, &myset);

	  if (select(sockfd + 1, NULL, &myset, NULL, &tv) > 0)
	  {
	    lon = sizeof(int);
	    getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
	    if (valopt)
	    {
	      std::cout << "Peer : " << strerror(errno);
	      peer.dump();
	    }
	    else
	      std::cout << "connected !!!!!!!!!!!! " << std::endl;
	  }
	  else
	  {
	    std::cout << "Peer Timeout : ";
	    peer.dump();
	    std::cout << std::endl;
	  }
	}
	else
	  std::cout << strerror(errno);
      }

      // blocking socket
      fcntl(sockfd, F_SETFL, ~O_NONBLOCK);
    }
  }

  void Client::dumpPeers()
  {
    for (auto i : peers_)
      i.dump();
  }

} // namespace Network
