/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_encryptation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 18:20:56 by plamtenz          #+#    #+#             */
/*   Updated: 2020/02/29 18:20:56 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Will aes algorithm used by US Army to Top secret 
** archives. AES has his opensll lib so is "easy" to 
** implement
*/

//#include <openssl/evp.h>
//#include <openssl/aes.h>

typedef struct          s_aes
{
    char                *cypher_text;
    char                *plain_text;
    size_t              cypher_size; // xd i wrote bad cipher :/
    size_t              plain_size;
    char                error;
    EVP_CIPHER_CTX      *context_encrypt;
    EVP_CIPHER_CTX      *context_decrypt;
    char                *key;
    size_t              key_size;
    char                *init_vector;
    size_t              vec_size;
}                       t_aes;

#define KEY             "to define must be 256bits for better security"
#define IV              "to define must be 128 bits"

t_aes                   *init_aes(void)
{
    t_aes               *new;

    (void)OpenSSL_add_all_algorithms();
    // define context and init
    if (!(new = malloc(sizeof(t_aes)))
            || !(new->context_encrypt = EVP_CIPHER_CTX_new())
            || !(new->context_decrypt = EVP_CIPHER_CTX_new())
            || !EVP_EncryptInit_ex(new->context_encrypt, EVP_aes_256_gcm(), NULL, NULL, NULL)
            || !EVP_DecryptInit_ex(new->context_decrypt, EVP_aes_256_gcm(), NULL, NULL, NULL))
        return (NULL);
    // allocate key and iv 
    if (!(new->key = malloc(sizeof(char) * sizeof(KEY)))
            || !(new->init_vector = malloc(sizeof(char) * sizeof(IV))))
        return (NULL);
    new->key_size = sizeof(KEY);
    new->vec_size = sizeof(IV);
    const char          tmpkey = KEY;
    const char          tmpiv = IV;
    (void)memcpy(new->key, tmpkey, new->key_size);
    (void)memcpy(new->init_vector, tmpiv, new->vec_size);
    // init decryp and encrypt
    if (!(EVP_CIPHER_CTX_ctrl(new->context_encrypt, EVP_CTRL_GCM_SET_IVLEN, new->vec_size, NULL) == 1
            && EVP_EncryptInit_ex(new->context_encrypt, NULL, NULL, new->key, new->init_vector) == 1
            && EVP_CIPHER_CTX_ctrl(new->context_decrypt, EVP_CTRL_GCM_SET_IVLEN, aes->vec_size, NULL) == 1
            && EVP_DecryptInit_ex(new->context_decrypt, NULL, NULL, new->key, new->init_vector) == 1))
        return (NULL);
    new->error = 1;
    new->plain_text = NULL;
    new->cypher_text = NULL;
    new->cypher_size = 0;
    new->plain_size = 0;
    return (new);
}

char                encrypt(t_aes *aes, char *plain_text, size_t plain_size)
{
    int           size;

    size = 0;
    if (!(aes->plain_text = malloc(sizeof(char) * plain_size)))
        return (FAILURE);
    aes->plain_size = plain_size;
    (void)memcpy(aes->plain_text, plain_text, plain_size);
    // rm last cypher cause we will write another
    if (aes->cypher_text)
    {
        (void)free(aes->cypher_text);
        aes->cypher_text = NULL;
    }
    // encrypt with the lib not very hard :)
    if (!(aes->cypher_text = malloc(sizeof(char) * aes->plain_size))
            || !EVP_EncryptUpdate(aes->context_encrypt, aes->cypher_text, &size, aes->plain_text, aes->plain_size)
            || EVP_EncryptFinal_ex(aes->context_encrypt, aes->cypher_text + size, &size) != 1)
        return (FAILURE);
    aes->cypher_size = size;
    return (size > 0 ? SUCCESS : FAILURE);
}

char                decrypt(t_aes *aes, char *cipher_text, size_t cipher_size)
{
    int             size;

    size = 0;
    if (!(aes->cipher_text = malloc(sizeof(char) * cipher_size)))
        return (FAILURE);
    eas->cypher_size = cipher_size;
    (void)memcpy(aes->cypher_text, cipher_text, cipher_size);
    // rm las plain cause we will write another
    if (aes->plain_text)
    {
        (void)free(aes->plain_text);
        aes->plain_text = NULL;
    }
    // decrypt with the lib not very hard :)
    if (!(aes->plain_text = malloc(sizeof(char) * aes->cypher_size))
            || !EVP_DecryptUpdate(aes->context_decrypt, aes->text_plain, &size, aes->cypher_text, aes->cypher_size)
            || EVP_DecryptFinal_ex(aes->context_decrypt, aes->plain_text + size, &size))
        return (FAILURE);
    aes->plain_size = size;
    return (size > 0 ? SUCCESS : FAILURE);
}

void            free_aes(t_aes *aes)
{
    if (aes->key)
        (void)free(aes->key);
    if (aes->init_vector)
        (void)free(aes->init_vector);
    EVP_CIPHER_CTX_free(aes->context_encrypt);
	EVP_CIPHER_CTX_free(aes->context_decrypt);
    (void)free(aes);
}