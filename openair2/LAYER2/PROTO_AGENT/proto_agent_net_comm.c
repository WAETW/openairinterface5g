/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file enb_agent_net_comm.c
 * \brief enb agent network interface abstraction 
 * \author Xenofon Foukas
 * \date 2016
 * \version 0.1
 */

#include "proto_agent_net_comm.h"
#include "common/utils/LOG/log.h"

proto_agent_channel_t *proto_channel[NUMBER_OF_eNB_MAX][ENB_AGENT_MAX];
static proto_agent_channel_instance_t channel_instance;
int proto_agent_channel_id = 0;

int proto_agent_register_channel(mod_id_t mod_id, proto_agent_channel_t *channel, proto_agent_id_t agent_id) {
  int i;

  if (channel == NULL) {
    return -1;
  }

  if (agent_id == ENB_AGENT_MAX) {
    for (i = 0; i < ENB_AGENT_MAX; i++) {
      proto_channel[mod_id][i] = channel;
    }
  } else {
    proto_channel[mod_id][agent_id] = channel;
  }
  return 0;
}

void proto_agent_unregister_channel(mod_id_t mod_id, proto_agent_id_t agent_id) {
  int i;

  if (agent_id == ENB_AGENT_MAX) {
    for (i = 0; i < ENB_AGENT_MAX; i++) {
      proto_channel[mod_id][i] = NULL;
    }
  } else {
    proto_channel[mod_id][agent_id] = NULL;
  }
}

int proto_agent_create_channel(void *channel_info,
			       int (*msg_send)(void *data, int size, int priority, void *channel_info),
			       int (*msg_recv)(void **data, int *priority, void *channel_info),
			     void (*release)(proto_agent_channel_t *channel)) {
  
  int channel_id = ++proto_agent_channel_id;
  proto_agent_channel_t *channel = (proto_agent_channel_t *) malloc(sizeof(proto_agent_channel_t));
  channel->channel_id = channel_id;
  channel->channel_info = channel_info;
  channel->msg_send = msg_send;
  channel->msg_recv = msg_recv;
  channel->release = release;
  
  /*element should be a real pointer*/
  RB_INSERT(proto_agent_channel_map, &channel_instance.proto_agent_head, channel); 
  
  LOG_D(PROTO_AGENT, "Created a new channel with id 0x%x\n", channel->channel_id);
 
  return channel_id; 
}

int proto_agent_destroy_channel(int channel_id) {
  int i, j;

  /*Check to see if channel exists*/
  struct proto_agent_channel_s *e = NULL;
  struct proto_agent_channel_s search;
  memset(&search, 0, sizeof(struct proto_agent_channel_s));

  e = RB_FIND(proto_agent_channel_map, &channel_instance.proto_agent_head, &search);

  if (e == NULL) {
    return -1;
  }

  /*Unregister the channel from all agents*/
  for (i = 0; i < NUMBER_OF_eNB_MAX; i++) {
    for (j = 0; j < ENB_AGENT_MAX; j++) {
      if (proto_channel[i][j] != NULL) {
	if (proto_channel[i][j]->channel_id == e->channel_id) {
	  proto_channel[i][j] = NULL;
	}
      }
    }
  }

  /*Remove the channel from the tree and free memory*/
  RB_REMOVE(proto_agent_channel_map, &channel_instance.proto_agent_head, e);
  e->release(e);
  free(e);

  return 0;
}

err_code_t proto_agent_init_channel_container(void) {
  int i, j;
  LOG_D(PROTO_AGENT, "init RB tree for channel container\n");

  RB_INIT(&channel_instance.proto_agent_head);

  for (i = 0; i < NUMBER_OF_eNB_MAX; i++) {
    for (j = 0; j < ENB_AGENT_MAX; j++) {
    proto_channel[i][j] = NULL;
    }
  }

  return 0;
}

RB_GENERATE(proto_agent_channel_map,proto_agent_channel_s, entry, proto_agent_compare_channel);

int proto_agent_compare_channel(struct proto_agent_channel_s *a, struct proto_agent_channel_s *b) {
  if (a->channel_id < b->channel_id) return -1;
  if (a->channel_id > b->channel_id) return 1;

  // equal timers
  return 0;
}

proto_agent_channel_t * proto_agent_get_channel(int channel_id) {
  
  struct proto_agent_channel_s search;
  memset(&search, 0, sizeof(struct proto_agent_channel_s));
  search.channel_id = channel_id;
  
  return  RB_FIND(proto_agent_channel_map, &channel_instance.proto_agent_head, &search);
  
}
