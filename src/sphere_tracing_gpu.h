#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <array>

#include "vk_pipeline.h"
#include "vk_buffers.h"
#include "vk_utils.h"
#include "vk_copy.h"
#include "vk_context.h"


#include "sphere_tracing.h"

#include "include/SphereTracer_gpu_ubo.h"
class SphereTracer_GPU : public SphereTracer
{
public:

  SphereTracer_GPU() 
  {
  }
  virtual void InitVulkanObjects(VkDevice a_device, VkPhysicalDevice a_physicalDevice, size_t a_maxThreadsCount);
  
  virtual void SetVulkanContext(vk_utils::VulkanContext a_ctx) { m_ctx = a_ctx; }
  virtual void SetVulkanInOutFor_draw(
    VkBuffer sceneBuffer,
    size_t   sceneOffset,
    VkBuffer imageBuffer,
    size_t   imageOffset,
    uint32_t dummyArgument = 0)
  {
    draw_local.sceneBuffer = sceneBuffer;
    draw_local.sceneOffset = sceneOffset;
    draw_local.imageBuffer = imageBuffer;
    draw_local.imageOffset = imageOffset;
    InitAllGeneratedDescriptorSets_draw();
  }

  virtual ~SphereTracer_GPU();

  
  virtual void InitMemberBuffers();
  virtual void UpdateAll(std::shared_ptr<vk_utils::ICopyEngine> a_pCopyEngine)
  {
    UpdatePlainMembers(a_pCopyEngine);
    UpdateVectorMembers(a_pCopyEngine);
    UpdateTextureMembers(a_pCopyEngine);
  }
  
  virtual void CommitDeviceData(std::shared_ptr<vk_utils::ICopyEngine> a_pCopyHelper) // you have to define this virtual function in the original imput class
  {
    InitMemberBuffers();
    UpdateAll(a_pCopyHelper);
  }  
  void CommitDeviceData() override { CommitDeviceData(m_ctx.pCopyHelper); }  
  void GetExecutionTime(const char* a_funcName, float a_out[4]) override; 
  

  virtual void ReserveEmptyVectors();
  virtual void UpdatePlainMembers(std::shared_ptr<vk_utils::ICopyEngine> a_pCopyEngine);
  virtual void UpdateVectorMembers(std::shared_ptr<vk_utils::ICopyEngine> a_pCopyEngine);
  virtual void UpdateTextureMembers(std::shared_ptr<vk_utils::ICopyEngine> a_pCopyEngine);
  virtual void ReadPlainMembers(std::shared_ptr<vk_utils::ICopyEngine> a_pCopyEngine);
  static VkPhysicalDeviceFeatures2 ListRequiredDeviceFeatures(std::vector<const char*>& deviceExtensions);
  
  virtual void drawCmd(VkCommandBuffer a_commandBuffer, const float* scene, uint count, float* image, const Camera camera, const DirectedLight light, uint width, uint height);

  void draw(const float* scene, uint count, float* image, const Camera camera, const DirectedLight light, uint width, uint height) override;

  inline vk_utils::ExecTime GetdrawExecutionTime() const { return m_exTimedraw; }

  vk_utils::ExecTime m_exTimedraw;

  virtual void copyKernelFloatCmd(uint32_t length);
  
  virtual void drawCmd(const float* scene, uint count, float* image, const Camera camera, const DirectedLight dir_light, uint width, uint height);
  
  struct MemLoc
  {
    VkDeviceMemory memObject = VK_NULL_HANDLE;
    size_t         memOffset = 0;
    size_t         allocId   = 0;
  };

  virtual MemLoc AllocAndBind(const std::vector<VkBuffer>& a_buffers); ///< replace this function to apply custom allocator
  virtual MemLoc AllocAndBind(const std::vector<VkImage>& a_image);    ///< replace this function to apply custom allocator
  virtual void   FreeAllAllocations(std::vector<MemLoc>& a_memLoc);    ///< replace this function to apply custom allocator

protected:

  VkPhysicalDevice           physicalDevice = VK_NULL_HANDLE;
  VkDevice                   device         = VK_NULL_HANDLE;
  vk_utils::VulkanContext    m_ctx          = {};
  VkCommandBuffer            m_currCmdBuffer   = VK_NULL_HANDLE;
  uint32_t                   m_currThreadFlags = 0;
  std::vector<MemLoc>        m_allMems;
  VkPhysicalDeviceProperties m_devProps;

  VkBufferMemoryBarrier BarrierForClearFlags(VkBuffer a_buffer);
  VkBufferMemoryBarrier BarrierForSingleBuffer(VkBuffer a_buffer);
  void BarriersForSeveralBuffers(VkBuffer* a_inBuffers, VkBufferMemoryBarrier* a_outBarriers, uint32_t a_buffersNum);

  virtual void InitHelpers();
  virtual void InitBuffers(size_t a_maxThreadsCount, bool a_tempBuffersOverlay = true);
  virtual void InitKernels(const char* a_filePath);
  virtual void AllocateAllDescriptorSets();

  virtual void InitAllGeneratedDescriptorSets_draw();

  virtual void AssignBuffersToMemory(const std::vector<VkBuffer>& a_buffers, VkDeviceMemory a_mem);

  virtual void AllocMemoryForMemberBuffersAndImages(const std::vector<VkBuffer>& a_buffers, const std::vector<VkImage>& a_image);
  virtual std::string AlterShaderPath(const char* in_shaderPath) { return std::string("") + std::string(in_shaderPath); }

  
  

  struct draw_Data
  {
    VkBuffer sceneBuffer = VK_NULL_HANDLE;
    size_t   sceneOffset = 0;
    VkBuffer imageBuffer = VK_NULL_HANDLE;
    size_t   imageOffset = 0;
    bool needToClearOutput = false;
  } draw_local;



  struct MembersDataGPU
  {
  } m_vdata;
  
  
  size_t m_maxThreadCount = 0;
  VkBuffer m_classDataBuffer = VK_NULL_HANDLE;

  VkPipelineLayout      drawLayout   = VK_NULL_HANDLE;
  VkPipeline            drawPipeline = VK_NULL_HANDLE; 
  VkDescriptorSetLayout drawDSLayout = VK_NULL_HANDLE;
  VkDescriptorSetLayout CreatedrawDSLayout();
  virtual void InitKernel_draw(const char* a_filePath);


  virtual VkBufferUsageFlags GetAdditionalFlagsForUBO() const;
  virtual uint32_t           GetDefaultMaxTextures() const;

  VkPipelineLayout      copyKernelFloatLayout   = VK_NULL_HANDLE;
  VkPipeline            copyKernelFloatPipeline = VK_NULL_HANDLE;
  VkDescriptorSetLayout copyKernelFloatDSLayout = VK_NULL_HANDLE;
  VkDescriptorSetLayout CreatecopyKernelFloatDSLayout();

  VkDescriptorPool m_dsPool = VK_NULL_HANDLE;
  VkDescriptorSet  m_allGeneratedDS[1];

  SphereTracer_GPU_UBO_Data m_uboData;
  
  constexpr static uint32_t MEMCPY_BLOCK_SIZE = 256;
  constexpr static uint32_t REDUCTION_BLOCK_SIZE = 256;

  virtual void MakeComputePipelineAndLayout(const char* a_shaderPath, const char* a_mainName, const VkSpecializationInfo *a_specInfo, const VkDescriptorSetLayout a_dsLayout, 
                                            VkPipelineLayout* pPipelineLayout, VkPipeline* pPipeline);
  virtual void MakeComputePipelineOnly(const char* a_shaderPath, const char* a_mainName, const VkSpecializationInfo *a_specInfo, const VkDescriptorSetLayout a_dsLayout, VkPipelineLayout pipelineLayout, 
                                       VkPipeline* pPipeline);

  std::vector<VkPipelineLayout> m_allCreatedPipelineLayouts; ///<! remenber them here to delete later
  std::vector<VkPipeline>       m_allCreatedPipelines;       ///<! remenber them here to delete later
public:

  struct MegaKernelIsEnabled
  {
    bool dummy = 0;
  };

  static MegaKernelIsEnabled  m_megaKernelFlags;
  static MegaKernelIsEnabled& EnabledPipelines() { return m_megaKernelFlags; }

};


