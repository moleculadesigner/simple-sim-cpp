.PHONY: debug release clean

debug:
	cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
	cmake --build build-debug

release:
	cmake -B build-release -DCMAKE_BUILD_TYPE=Release
	cmake --build build-release

# Только обновление json для Zed/clangd без полной пересборки
commands:
	cmake -B build-debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	ln -sf build-debug/compile_commands.json .

clean:
	rm -rf build-debug build-release
