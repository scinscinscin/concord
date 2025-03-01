#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "discord.h"

void
print_usage(void)
{
    printf("\n\nThis bot demonstrates how easy it is to setup a bot that"
           " copies user actions.\n"
           "1. Send a message in any chat\n"
           "2. Edit that message\n"
           "3. Delete that message\n"
           "4. Add a reaction to a message\n"
           "5. Have another bot bulk-delete messages\n"
           "\nTYPE ANY KEY TO START BOT\n");
}

void
on_ready(struct discord *client)
{
    const struct discord_user *bot = discord_get_self(client);

    log_info("Copycat-Bot succesfully connected to Discord as %s#%s!",
             bot->username, bot->discriminator);
}

void
on_reaction_add(struct discord *client,
                u64snowflake user_id,
                u64snowflake channel_id,
                u64snowflake message_id,
                u64snowflake guild_id,
                const struct discord_guild_member *member,
                const struct discord_emoji *emoji)
{
    if (member->user->bot) return;

    discord_create_reaction(client, channel_id, message_id, emoji->id,
                            emoji->name, NULL);
}

void
on_message_create(struct discord *client, const struct discord_message *msg)
{
    if (msg->author->bot) return;

    struct discord_create_message params = { 
                   .content = msg->content,
                   .message_reference = !msg->referenced_message
                           ? NULL
                           : &(struct discord_message_reference){
                               .message_id = msg->referenced_message->id,
                               .channel_id = msg->channel_id,
                               .guild_id = msg->guild_id,
                           },
                    };

    discord_create_message(client, msg->channel_id, &params, NULL);
}

void
on_message_update(struct discord *client, const struct discord_message *msg)
{
    if (msg->author->bot) return;

    struct discord_create_message params = { .content =
                                                 "I see what you did there." };
    discord_create_message(client, msg->channel_id, &params, NULL);
}

void
on_message_delete(struct discord *client,
                  u64snowflake id,
                  u64snowflake channel_id,
                  u64snowflake guild_id)
{
    struct discord_create_message params = {
        .content = "Did that message just disappear?"
    };
    discord_create_message(client, channel_id, &params, NULL);
}

void
on_message_delete_bulk(struct discord *client,
                       const struct snowflakes *ids,
                       u64snowflake channel_id,
                       u64snowflake guild_id)
{
    char text[128];
    sprintf(text, "Where did those %d messages go?", ids->size);

    struct discord_create_message params = { .content = text };
    discord_create_message(client, channel_id, &params, NULL);
}

int
main(int argc, char *argv[])
{
    const char *config_file;
    if (argc > 1)
        config_file = argv[1];
    else
        config_file = "../config.json";

    ccord_global_init();
    struct discord *client = discord_config_init(config_file);
    assert(NULL != client && "Couldn't initialize client");

    discord_set_on_ready(client, &on_ready);
    discord_set_on_message_create(client, &on_message_create);
    discord_set_on_message_update(client, &on_message_update);
    discord_set_on_message_delete(client, &on_message_delete);
    discord_set_on_message_reaction_add(client, &on_reaction_add);
    discord_set_on_message_delete_bulk(client, &on_message_delete_bulk);

    print_usage();
    fgetc(stdin); // wait for input

    discord_run(client);

    discord_cleanup(client);
    ccord_global_cleanup();
}
