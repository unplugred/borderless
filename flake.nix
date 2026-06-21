{
	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
		flake-utils.url = "github:numtide/flake-utils";
	};

	outputs = { self, nixpkgs, flake-utils, ... }:
	flake-utils.lib.eachDefaultSystem (system:
	let
		pkgs = nixpkgs.legacyPackages.${system};
		pythonPackages = ps: with ps; [ ]; # python packages
		pythonEnv = pkgs.python311.withPackages pythonPackages;
	in {
		devShells.default = pkgs.mkShell {
			buildInputs = with pkgs; [ # packages
				git
				pythonEnv
				cmake
				qt6.qtbase
				qt6.qtimageformats
				qt6.qtsvg
				gdb
			];

			# environment variables
			PROJECT_NAME = "borderless";
		};
	});
}
