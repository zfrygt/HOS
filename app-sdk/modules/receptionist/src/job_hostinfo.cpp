#include <job_hostinfo.h>
#include <assert.h>
#include <hos_protocol.pb.h>
#include <module_status.h>
#include <module_connector.h>

JobHostInfo::JobHostInfo(ModuleConnector* module_connector):
m_module_connector(module_connector)
{
	assert(m_module_connector != nullptr);
}

void JobHostInfo::execute()
{
	auto client_message = std::make_unique<ClientMessage>();
	client_message->set_type(ClientMessage_Type_HostInfo);
	client_message->mutable_host_info()->set_total_cpu(HardwareStatus::get_total_cpu());
	client_message->mutable_host_info()->set_total_ram(HardwareStatus::get_total_mem());
	client_message->mutable_host_info()->set_total_disk(HardwareStatus::get_total_dsk());
	Envelope<::google::protobuf::Message> envelope(std::move(client_message));
	m_module_connector->send(&envelope);
}