#include "core/torrent-manager.hh"

int main(void)
{
  Core::TorrentManager torrentManager;
  torrentManager.download("tests/l.torrent");
  //client.download("tests/secretFamilyRecipes.torrent");

  return 0;
}
