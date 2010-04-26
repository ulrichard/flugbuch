#!/usr/bin/perl -w

################################################################
# Include the flugbuch2 settings int /etc/wt/wt_config.xml
################################################################

use strict;
use XML::Twig;

my $twig  = new XML::Twig;
$twig->parsefile("/etc/wt/wt_config.xml");
my $root = $twig->root;

my $twigm = new XML::Twig;
$twigm->parsefile("/usr/share/flugbuch2/wt_config_merge.xml");
my $rootm = $twigm->root;
$rootm->paste('last_child', $root);

$twig->print;

