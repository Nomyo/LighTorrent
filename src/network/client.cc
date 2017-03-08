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

    Network::Torrent torrent(getType<BType_ptr, BDico>(decodedFile));

    Core::URLUtils url;
    std::string urlGenerated = url.generateURL(torrent);
    UrlParser::UrlParser up(urlGenerated);
    up.dump();
    std::cout << std::endl;

    TrackerConnector::TrackerConnector tc(&torrent);
    std::string result = tc.announce(urlGenerated);
    auto result_node = getType<BType_ptr, BDico>(driver.bDecode(result));
    std::string peersBinary = getDecode<BType_ptr, BString, std::string>(result_node.get("peers"));

    getPeersFromBinary(peersBinary);
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
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in remoteaddr;
    remoteaddr.sin_family = AF_INET;
    remoteaddr.sin_addr.s_addr = inet_addr(peers_[0].getIp().c_str());
    remoteaddr.sin_port = peers_[0].getPort();

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv)) < 0)
      std::cerr << "Could not set timeout to udp socket..." << std::endl;

    if (connect(sockfd, (struct sockaddr *)&remoteaddr, sizeof(remoteaddr)) < 0)
    {
      std::cout << "failed to connect to peers : ";
      peers_[0].dump();
      std::cout << std::endl;
    }
    else
      std::cout << "YEAEHAEHAYEHAEYAEH" << std::endl;

    close(sockfd);
  }

  void Client::dumpPeers()
  {
    for (auto i : peers_)
      i.dump();
  }

} // namespace Network
