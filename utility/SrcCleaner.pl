#!perl

use Getopt::Long;
use File::Find;
use Text::Tabs;
use File::Copy qw(copy);
use File::Basename;

my $dirname = dirname(__FILE__);
my $toTab	= 0;
$tabstop	= 4;

GetOptions
(
	"tabulate"	=> \$toTab,
	"tabsize=i"	=> \$tabstop
)
or die "Error in command line arguments!\n";

unless( chdir $dirname )
{
	die "Can't change to utility directory!\n( $dirname )\n";
}

@paths = ( "../szakdolgozat/" );
@files = ();

find(\&wanted, @paths);

sub wanted
{
	if( $_ =~ /.*\.[ch](?:pp)?$/i )
	{
		push @files, $File::Find::name;
	}
}

@files = sort @files;

$mode = $toTab ? '>>' : '>';
if( open(LOG, $mode, 'SrcClean.log') )
{
	if( $toTab )
	{
		print LOG "\n" . '-' x 50 . "\n\n";
	}
	
	foreach $file ( @files )
	{
		print LOG "F: $file\n";
		
		if( -e $file . '.tmp' ) 
		{
			unlink $file . '.tmp';
		}
		
		if( copy($file, $file . '.tmp') )
		{
			if( open(SRC, '<', $file) )
			{
				@lines = <SRC>;
				close SRC;
				
				foreach my $line ( @lines )
				{
					$line =~ s/\s*$//;
					$line =~ s/\s+$//;
				}
				
				while( $lines[-1] eq "" )
				{
					pop @lines;
				}
				
				@lines = $toTab ? unexpand( @lines ) : expand( @lines );
				
				push @lines, "";

				if( open(SRC, '>', $file) )
				{
					if( print SRC join( "\n", @lines ) )
					{
						print LOG "I: File reformatted!\n";
						close SRC;
					}
					else
					{
						print LOG "E: Can't write to file! Trying to restore it!\n";
						close SRC;
						
						if( -e $file )
						{
							unlink $file;
						}
						
						if( copy($file . '.tmp', $file) )
						{
							print LOG "I: File restored!\n";
							unlink $file . '.tmp';
						}
						else
						{
							print LOG "E: Can't restore file!\n";
						}
					}
				}
				else
				{
					print LOG "E: Can't open file for WRITE! Skipping!\n"
				}
			}
			else
			{
				print LOG "E: Can't open file for READ! Skipping!\n"
			}
		}
		else
		{
			print LOG "E: Can't create backup! Skipping!\n"
		}
	}
	
	close LOG;
}
else
{
	die "Can't open log file!\n";
}
