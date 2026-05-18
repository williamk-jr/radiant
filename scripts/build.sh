source ~/sdk/VulkanSdk/setup-env.sh
#echo $VK_ADD_LAYER_PATH
export VK_INSTANCE_LAYERS=VK_LAYER_KHRONOS_validation

cmake -S . -B build -G "Ninja"
ninja -C build
