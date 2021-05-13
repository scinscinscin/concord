#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <pthread.h>
#include <assert.h>

#include "discord.h"


/* Save ID so we don't have to specify it everytime */
u64_snowflake_t curr_dm_channel_id;

void on_ready(struct discord *client, const struct discord_user *bot) {
  fprintf(stderr, "\n\nManualDM-Bot succesfully connected to Discord as %s#%s!\n\n",
      bot->username, bot->discriminator);
}

void on_dm_receive(
    struct discord *client,
    const struct discord_user *bot,
    const struct discord_message *msg)
{
  if (msg->author->bot) return;

  struct discord_channel *dm_channel = discord_channel_alloc();
  discord_get_channel(client, msg->channel_id, dm_channel);
  if (dm_channel->type != DISCORD_CHANNEL_DM) {
    discord_channel_free(dm_channel);
    return;
  }
  fprintf(stdout, "%s:%s\n", msg->author->username, msg->content);
  discord_channel_free(dm_channel);
}

void* read_input(void *p_client)
{
  pthread_detach(pthread_self());
  struct discord *client = p_client;

  char buf[32 + MAX_MESSAGE_LEN];
  u64_snowflake_t recipient_id;
  char msg[MAX_MESSAGE_LEN];
  while (1) {
    memset(buf, 0, sizeof(buf));
    fgets(buf, sizeof(buf), stdin);
    if (!*buf) continue; // is empty
    
    memset(msg, 0, sizeof(msg));
    recipient_id=0;
    sscanf(buf, "%"PRIu64":%[^\n]", &recipient_id, msg);
    if (!recipient_id || !*msg) {
      sscanf(buf, "%[^\n]", msg);
      if (!*msg) {
        fprintf(stderr, "Expected format: <*recipient_id>:<message>"); 
        continue;
      }
    }
    else { /* reset active chat */
      struct discord_channel *dm_channel = discord_channel_alloc();
      discord_create_dm(client, recipient_id, dm_channel);
      curr_dm_channel_id = dm_channel->id;
      discord_channel_free(dm_channel);
    }
    struct discord_create_message_params params = { .content = msg };
    discord_create_message(client, curr_dm_channel_id, &params, NULL);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  const char *config_file;
  if (argc > 1)
    config_file = argv[1];
  else
    config_file = "bot.config";

  discord_global_init();

  struct discord *client = discord_config_init(config_file);
  assert(NULL != client && "Couldn't initialize client");

  discord_set_on_ready(client, &on_ready);
  discord_set_on_message_create(client, &on_dm_receive);

  printf("\n\nThis bot demonstrates how easy it is to start a DM"
         " with someone and talk without leaving the terminal\n" 
         "1. Type at the terminal <recipient_id>:<message> to start your conversation\n"
         "\tex: 1232232312321232123:Hello there friend!\n"
         "2. For successive messages to the same person, you can just type the message"
         " without the need of specifying the recipient_id everytime\n"
         "3. If you wish to start a new conversation, repeat the #1 format\n"
         "\nTYPE ANY KEY TO START BOT\n");
  fgetc(stdin); // wait for input

  pthread_t tid;
  pthread_create(&tid, NULL, &read_input, client);

  discord_run(client);

  discord_cleanup(client);

  discord_global_cleanup();
}
