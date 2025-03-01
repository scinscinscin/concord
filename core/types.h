/** @file types.h */

#ifndef CONCORD_TYPES_H
#define CONCORD_TYPES_H

/** @defgroup ConcordTypes Primitives
 *  @brief Commonly used datatypes
 *
 *  @note these datatypes are typedefs of C primitives,
 *          its purpose is to facilitate identification
 *          and "intent of use".
 *  @{ */

/**
 * @brief Unix time in milliseconds
 *
 * Commonly used for fields that may store timestamps
 */
typedef uint64_t u64unix_ms;
/**
 * @brief Snowflake datatype
 *
 * Used in APIs such as Twitter and Discord for their unique IDs
 */
typedef uint64_t u64snowflake;

/**
 * @brief Bitmask primitive
 *
 * Used for fields that may store values of, or perform bitwise operations
 */
typedef uint64_t u64bitmask;

/**
 * @brief Raw JSON string
 *
 * Used for fields that have dynamic or unreliable types. A string made out of
 *        `json_char` should be used to keep a raw JSON, which can then be
 * parsed with the assistance of a JSON library.
 */
typedef char json_char;

/** @} ConcordTypes */

#endif /* CONCORD_TYPES_H */
