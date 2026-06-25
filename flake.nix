{
	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
		flake-utils.url = "github:numtide/flake-utils";
	};

	outputs = { self, nixpkgs, flake-utils, ... }:
	flake-utils.lib.eachDefaultSystem (system:
	let
		pkgs = nixpkgs.legacyPackages.${system};
	in with pkgs; {
		devShells.default = pkgs.mkShell {
			buildInputs = [ # packages
				git
				(python3.withPackages (ps: with ps; [ # python packages
				]))
				cmake
				qt6.qtbase
				qt6.qtimageformats
				qt6.qtsvg
				gdb
			];

			# environment variables
			PROJECT_NAME = "borderless";
		};
		packages.default = stdenv.mkDerivation rec {
			name = "borderless";
			version = "2.0.0";
			src = ./.;

			buildInputs = [
				qt6.qtimageformats
				qt6.qtsvg
			];
			nativeBuildInputs = [
				qt6.qtbase
				qt6.wrapQtAppsHook
				cmake
			];

			installPhase = ''
				mkdir -p $out/bin
				cp borderless $out/bin
			'';
		};
	});
}
