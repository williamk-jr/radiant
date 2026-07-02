if [[ ! -v VK_ADD_LAYER_PATH ]]; then
  source ~/sdk/VulkanSdk/setup-env.sh
fi

heaptrack ~/Projects/C++/Radiant/build/Radiant
