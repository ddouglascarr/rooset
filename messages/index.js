/*eslint-disable block-scoped-var, id-length, no-control-regex, no-magic-numbers, no-prototype-builtins, no-redeclare, no-shadow, no-var, sort-vars*/
"use strict";

var $protobuf = require("protobufjs/minimal");

// Common aliases
var $util = $protobuf.util;

// Exported root namespace
var $root = $protobuf.roots["default"] || ($protobuf.roots["default"] = {});

$root.messages = (function() {

    /**
     * Namespace messages.
     * @exports messages
     * @namespace
     */
    var messages = {};

    messages.Blob = (function() {

        /**
         * Properties of a Blob.
         * @memberof messages
         * @interface IBlob
         * @property {string|null} [SHA] Blob SHA
         * @property {string|null} [Content] Blob Content
         */

        /**
         * Constructs a new Blob.
         * @memberof messages
         * @classdesc Represents a Blob.
         * @implements IBlob
         * @constructor
         * @param {messages.IBlob=} [properties] Properties to set
         */
        function Blob(properties) {
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * Blob SHA.
         * @member {string} SHA
         * @memberof messages.Blob
         * @instance
         */
        Blob.prototype.SHA = "";

        /**
         * Blob Content.
         * @member {string} Content
         * @memberof messages.Blob
         * @instance
         */
        Blob.prototype.Content = "";

        /**
         * Verifies a Blob message.
         * @function verify
         * @memberof messages.Blob
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        Blob.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                if (!$util.isString(message.SHA))
                    return "SHA: string expected";
            if (message.Content != null && message.hasOwnProperty("Content"))
                if (!$util.isString(message.Content))
                    return "Content: string expected";
            return null;
        };

        /**
         * Creates a Blob message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.Blob
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.Blob} Blob
         */
        Blob.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.Blob)
                return object;
            var message = new $root.messages.Blob();
            if (object.SHA != null)
                message.SHA = String(object.SHA);
            if (object.Content != null)
                message.Content = String(object.Content);
            return message;
        };

        /**
         * Creates a plain object from a Blob message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.Blob
         * @static
         * @param {messages.Blob} message Blob
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        Blob.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.defaults) {
                object.SHA = "";
                object.Content = "";
            }
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                object.SHA = message.SHA;
            if (message.Content != null && message.hasOwnProperty("Content"))
                object.Content = message.Content;
            return object;
        };

        /**
         * Converts this Blob to JSON.
         * @function toJSON
         * @memberof messages.Blob
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        Blob.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return Blob;
    })();

    /**
     * FileActionType enum.
     * @name messages.FileActionType
     * @enum {string}
     * @property {number} Create=0 Create value
     * @property {number} Update=1 Update value
     */
    messages.FileActionType = (function() {
        var valuesById = {}, values = Object.create(valuesById);
        values[valuesById[0] = "Create"] = 0;
        values[valuesById[1] = "Update"] = 1;
        return values;
    })();

    messages.FileAction = (function() {

        /**
         * Properties of a FileAction.
         * @memberof messages
         * @interface IFileAction
         * @property {messages.FileActionType|null} [Action] FileAction Action
         * @property {string|null} [FileName] FileAction FileName
         * @property {string|null} [Content] FileAction Content
         */

        /**
         * Constructs a new FileAction.
         * @memberof messages
         * @classdesc Represents a FileAction.
         * @implements IFileAction
         * @constructor
         * @param {messages.IFileAction=} [properties] Properties to set
         */
        function FileAction(properties) {
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * FileAction Action.
         * @member {messages.FileActionType} Action
         * @memberof messages.FileAction
         * @instance
         */
        FileAction.prototype.Action = 0;

        /**
         * FileAction FileName.
         * @member {string} FileName
         * @memberof messages.FileAction
         * @instance
         */
        FileAction.prototype.FileName = "";

        /**
         * FileAction Content.
         * @member {string} Content
         * @memberof messages.FileAction
         * @instance
         */
        FileAction.prototype.Content = "";

        /**
         * Verifies a FileAction message.
         * @function verify
         * @memberof messages.FileAction
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        FileAction.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.Action != null && message.hasOwnProperty("Action"))
                switch (message.Action) {
                default:
                    return "Action: enum value expected";
                case 0:
                case 1:
                    break;
                }
            if (message.FileName != null && message.hasOwnProperty("FileName"))
                if (!$util.isString(message.FileName))
                    return "FileName: string expected";
            if (message.Content != null && message.hasOwnProperty("Content"))
                if (!$util.isString(message.Content))
                    return "Content: string expected";
            return null;
        };

        /**
         * Creates a FileAction message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.FileAction
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.FileAction} FileAction
         */
        FileAction.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.FileAction)
                return object;
            var message = new $root.messages.FileAction();
            switch (object.Action) {
            case "Create":
            case 0:
                message.Action = 0;
                break;
            case "Update":
            case 1:
                message.Action = 1;
                break;
            }
            if (object.FileName != null)
                message.FileName = String(object.FileName);
            if (object.Content != null)
                message.Content = String(object.Content);
            return message;
        };

        /**
         * Creates a plain object from a FileAction message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.FileAction
         * @static
         * @param {messages.FileAction} message FileAction
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        FileAction.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.defaults) {
                object.Action = options.enums === String ? "Create" : 0;
                object.FileName = "";
                object.Content = "";
            }
            if (message.Action != null && message.hasOwnProperty("Action"))
                object.Action = options.enums === String ? $root.messages.FileActionType[message.Action] : message.Action;
            if (message.FileName != null && message.hasOwnProperty("FileName"))
                object.FileName = message.FileName;
            if (message.Content != null && message.hasOwnProperty("Content"))
                object.Content = message.Content;
            return object;
        };

        /**
         * Converts this FileAction to JSON.
         * @function toJSON
         * @memberof messages.FileAction
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        FileAction.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return FileAction;
    })();

    messages.CommitRecord = (function() {

        /**
         * Properties of a CommitRecord.
         * @memberof messages
         * @interface ICommitRecord
         * @property {string|null} [SHA] CommitRecord SHA
         * @property {string|null} [BranchName] CommitRecord BranchName
         */

        /**
         * Constructs a new CommitRecord.
         * @memberof messages
         * @classdesc Represents a CommitRecord.
         * @implements ICommitRecord
         * @constructor
         * @param {messages.ICommitRecord=} [properties] Properties to set
         */
        function CommitRecord(properties) {
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * CommitRecord SHA.
         * @member {string} SHA
         * @memberof messages.CommitRecord
         * @instance
         */
        CommitRecord.prototype.SHA = "";

        /**
         * CommitRecord BranchName.
         * @member {string} BranchName
         * @memberof messages.CommitRecord
         * @instance
         */
        CommitRecord.prototype.BranchName = "";

        /**
         * Verifies a CommitRecord message.
         * @function verify
         * @memberof messages.CommitRecord
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        CommitRecord.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                if (!$util.isString(message.SHA))
                    return "SHA: string expected";
            if (message.BranchName != null && message.hasOwnProperty("BranchName"))
                if (!$util.isString(message.BranchName))
                    return "BranchName: string expected";
            return null;
        };

        /**
         * Creates a CommitRecord message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.CommitRecord
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.CommitRecord} CommitRecord
         */
        CommitRecord.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.CommitRecord)
                return object;
            var message = new $root.messages.CommitRecord();
            if (object.SHA != null)
                message.SHA = String(object.SHA);
            if (object.BranchName != null)
                message.BranchName = String(object.BranchName);
            return message;
        };

        /**
         * Creates a plain object from a CommitRecord message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.CommitRecord
         * @static
         * @param {messages.CommitRecord} message CommitRecord
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        CommitRecord.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.defaults) {
                object.SHA = "";
                object.BranchName = "";
            }
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                object.SHA = message.SHA;
            if (message.BranchName != null && message.hasOwnProperty("BranchName"))
                object.BranchName = message.BranchName;
            return object;
        };

        /**
         * Converts this CommitRecord to JSON.
         * @function toJSON
         * @memberof messages.CommitRecord
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        CommitRecord.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return CommitRecord;
    })();

    messages.BlobRecord = (function() {

        /**
         * Properties of a BlobRecord.
         * @memberof messages
         * @interface IBlobRecord
         * @property {string|null} [SHA] BlobRecord SHA
         * @property {string|null} [Name] BlobRecord Name
         */

        /**
         * Constructs a new BlobRecord.
         * @memberof messages
         * @classdesc Represents a BlobRecord.
         * @implements IBlobRecord
         * @constructor
         * @param {messages.IBlobRecord=} [properties] Properties to set
         */
        function BlobRecord(properties) {
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * BlobRecord SHA.
         * @member {string} SHA
         * @memberof messages.BlobRecord
         * @instance
         */
        BlobRecord.prototype.SHA = "";

        /**
         * BlobRecord Name.
         * @member {string} Name
         * @memberof messages.BlobRecord
         * @instance
         */
        BlobRecord.prototype.Name = "";

        /**
         * Verifies a BlobRecord message.
         * @function verify
         * @memberof messages.BlobRecord
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        BlobRecord.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                if (!$util.isString(message.SHA))
                    return "SHA: string expected";
            if (message.Name != null && message.hasOwnProperty("Name"))
                if (!$util.isString(message.Name))
                    return "Name: string expected";
            return null;
        };

        /**
         * Creates a BlobRecord message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.BlobRecord
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.BlobRecord} BlobRecord
         */
        BlobRecord.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.BlobRecord)
                return object;
            var message = new $root.messages.BlobRecord();
            if (object.SHA != null)
                message.SHA = String(object.SHA);
            if (object.Name != null)
                message.Name = String(object.Name);
            return message;
        };

        /**
         * Creates a plain object from a BlobRecord message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.BlobRecord
         * @static
         * @param {messages.BlobRecord} message BlobRecord
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        BlobRecord.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.defaults) {
                object.SHA = "";
                object.Name = "";
            }
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                object.SHA = message.SHA;
            if (message.Name != null && message.hasOwnProperty("Name"))
                object.Name = message.Name;
            return object;
        };

        /**
         * Converts this BlobRecord to JSON.
         * @function toJSON
         * @memberof messages.BlobRecord
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        BlobRecord.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return BlobRecord;
    })();

    messages.NewInitiativeReq = (function() {

        /**
         * Properties of a NewInitiativeReq.
         * @memberof messages
         * @interface INewInitiativeReq
         * @property {Array.<messages.IFileAction>|null} [FileActions] NewInitiativeReq FileActions
         */

        /**
         * Constructs a new NewInitiativeReq.
         * @memberof messages
         * @classdesc Represents a NewInitiativeReq.
         * @implements INewInitiativeReq
         * @constructor
         * @param {messages.INewInitiativeReq=} [properties] Properties to set
         */
        function NewInitiativeReq(properties) {
            this.FileActions = [];
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * NewInitiativeReq FileActions.
         * @member {Array.<messages.IFileAction>} FileActions
         * @memberof messages.NewInitiativeReq
         * @instance
         */
        NewInitiativeReq.prototype.FileActions = $util.emptyArray;

        /**
         * Verifies a NewInitiativeReq message.
         * @function verify
         * @memberof messages.NewInitiativeReq
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        NewInitiativeReq.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.FileActions != null && message.hasOwnProperty("FileActions")) {
                if (!Array.isArray(message.FileActions))
                    return "FileActions: array expected";
                for (var i = 0; i < message.FileActions.length; ++i) {
                    var error = $root.messages.FileAction.verify(message.FileActions[i]);
                    if (error)
                        return "FileActions." + error;
                }
            }
            return null;
        };

        /**
         * Creates a NewInitiativeReq message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.NewInitiativeReq
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.NewInitiativeReq} NewInitiativeReq
         */
        NewInitiativeReq.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.NewInitiativeReq)
                return object;
            var message = new $root.messages.NewInitiativeReq();
            if (object.FileActions) {
                if (!Array.isArray(object.FileActions))
                    throw TypeError(".messages.NewInitiativeReq.FileActions: array expected");
                message.FileActions = [];
                for (var i = 0; i < object.FileActions.length; ++i) {
                    if (typeof object.FileActions[i] !== "object")
                        throw TypeError(".messages.NewInitiativeReq.FileActions: object expected");
                    message.FileActions[i] = $root.messages.FileAction.fromObject(object.FileActions[i]);
                }
            }
            return message;
        };

        /**
         * Creates a plain object from a NewInitiativeReq message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.NewInitiativeReq
         * @static
         * @param {messages.NewInitiativeReq} message NewInitiativeReq
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        NewInitiativeReq.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.arrays || options.defaults)
                object.FileActions = [];
            if (message.FileActions && message.FileActions.length) {
                object.FileActions = [];
                for (var j = 0; j < message.FileActions.length; ++j)
                    object.FileActions[j] = $root.messages.FileAction.toObject(message.FileActions[j], options);
            }
            return object;
        };

        /**
         * Converts this NewInitiativeReq to JSON.
         * @function toJSON
         * @memberof messages.NewInitiativeReq
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        NewInitiativeReq.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return NewInitiativeReq;
    })();

    messages.NewInitiativeResp = (function() {

        /**
         * Properties of a NewInitiativeResp.
         * @memberof messages
         * @interface INewInitiativeResp
         * @property {messages.ICommitRecord|null} [CommitRecord] NewInitiativeResp CommitRecord
         */

        /**
         * Constructs a new NewInitiativeResp.
         * @memberof messages
         * @classdesc Represents a NewInitiativeResp.
         * @implements INewInitiativeResp
         * @constructor
         * @param {messages.INewInitiativeResp=} [properties] Properties to set
         */
        function NewInitiativeResp(properties) {
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * NewInitiativeResp CommitRecord.
         * @member {messages.ICommitRecord|null|undefined} CommitRecord
         * @memberof messages.NewInitiativeResp
         * @instance
         */
        NewInitiativeResp.prototype.CommitRecord = null;

        /**
         * Verifies a NewInitiativeResp message.
         * @function verify
         * @memberof messages.NewInitiativeResp
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        NewInitiativeResp.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.CommitRecord != null && message.hasOwnProperty("CommitRecord")) {
                var error = $root.messages.CommitRecord.verify(message.CommitRecord);
                if (error)
                    return "CommitRecord." + error;
            }
            return null;
        };

        /**
         * Creates a NewInitiativeResp message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.NewInitiativeResp
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.NewInitiativeResp} NewInitiativeResp
         */
        NewInitiativeResp.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.NewInitiativeResp)
                return object;
            var message = new $root.messages.NewInitiativeResp();
            if (object.CommitRecord != null) {
                if (typeof object.CommitRecord !== "object")
                    throw TypeError(".messages.NewInitiativeResp.CommitRecord: object expected");
                message.CommitRecord = $root.messages.CommitRecord.fromObject(object.CommitRecord);
            }
            return message;
        };

        /**
         * Creates a plain object from a NewInitiativeResp message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.NewInitiativeResp
         * @static
         * @param {messages.NewInitiativeResp} message NewInitiativeResp
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        NewInitiativeResp.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.defaults)
                object.CommitRecord = null;
            if (message.CommitRecord != null && message.hasOwnProperty("CommitRecord"))
                object.CommitRecord = $root.messages.CommitRecord.toObject(message.CommitRecord, options);
            return object;
        };

        /**
         * Converts this NewInitiativeResp to JSON.
         * @function toJSON
         * @memberof messages.NewInitiativeResp
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        NewInitiativeResp.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return NewInitiativeResp;
    })();

    messages.GetBlobReq = (function() {

        /**
         * Properties of a GetBlobReq.
         * @memberof messages
         * @interface IGetBlobReq
         * @property {string|null} [SHA] GetBlobReq SHA
         */

        /**
         * Constructs a new GetBlobReq.
         * @memberof messages
         * @classdesc Represents a GetBlobReq.
         * @implements IGetBlobReq
         * @constructor
         * @param {messages.IGetBlobReq=} [properties] Properties to set
         */
        function GetBlobReq(properties) {
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * GetBlobReq SHA.
         * @member {string} SHA
         * @memberof messages.GetBlobReq
         * @instance
         */
        GetBlobReq.prototype.SHA = "";

        /**
         * Verifies a GetBlobReq message.
         * @function verify
         * @memberof messages.GetBlobReq
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        GetBlobReq.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                if (!$util.isString(message.SHA))
                    return "SHA: string expected";
            return null;
        };

        /**
         * Creates a GetBlobReq message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.GetBlobReq
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.GetBlobReq} GetBlobReq
         */
        GetBlobReq.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.GetBlobReq)
                return object;
            var message = new $root.messages.GetBlobReq();
            if (object.SHA != null)
                message.SHA = String(object.SHA);
            return message;
        };

        /**
         * Creates a plain object from a GetBlobReq message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.GetBlobReq
         * @static
         * @param {messages.GetBlobReq} message GetBlobReq
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        GetBlobReq.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.defaults)
                object.SHA = "";
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                object.SHA = message.SHA;
            return object;
        };

        /**
         * Converts this GetBlobReq to JSON.
         * @function toJSON
         * @memberof messages.GetBlobReq
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        GetBlobReq.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return GetBlobReq;
    })();

    messages.GetBlobResp = (function() {

        /**
         * Properties of a GetBlobResp.
         * @memberof messages
         * @interface IGetBlobResp
         * @property {messages.IBlob|null} [Blob] GetBlobResp Blob
         */

        /**
         * Constructs a new GetBlobResp.
         * @memberof messages
         * @classdesc Represents a GetBlobResp.
         * @implements IGetBlobResp
         * @constructor
         * @param {messages.IGetBlobResp=} [properties] Properties to set
         */
        function GetBlobResp(properties) {
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * GetBlobResp Blob.
         * @member {messages.IBlob|null|undefined} Blob
         * @memberof messages.GetBlobResp
         * @instance
         */
        GetBlobResp.prototype.Blob = null;

        /**
         * Verifies a GetBlobResp message.
         * @function verify
         * @memberof messages.GetBlobResp
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        GetBlobResp.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.Blob != null && message.hasOwnProperty("Blob")) {
                var error = $root.messages.Blob.verify(message.Blob);
                if (error)
                    return "Blob." + error;
            }
            return null;
        };

        /**
         * Creates a GetBlobResp message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.GetBlobResp
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.GetBlobResp} GetBlobResp
         */
        GetBlobResp.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.GetBlobResp)
                return object;
            var message = new $root.messages.GetBlobResp();
            if (object.Blob != null) {
                if (typeof object.Blob !== "object")
                    throw TypeError(".messages.GetBlobResp.Blob: object expected");
                message.Blob = $root.messages.Blob.fromObject(object.Blob);
            }
            return message;
        };

        /**
         * Creates a plain object from a GetBlobResp message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.GetBlobResp
         * @static
         * @param {messages.GetBlobResp} message GetBlobResp
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        GetBlobResp.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.defaults)
                object.Blob = null;
            if (message.Blob != null && message.hasOwnProperty("Blob"))
                object.Blob = $root.messages.Blob.toObject(message.Blob, options);
            return object;
        };

        /**
         * Converts this GetBlobResp to JSON.
         * @function toJSON
         * @memberof messages.GetBlobResp
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        GetBlobResp.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return GetBlobResp;
    })();

    messages.ListAreaBlobsReq = (function() {

        /**
         * Properties of a ListAreaBlobsReq.
         * @memberof messages
         * @interface IListAreaBlobsReq
         * @property {string|null} [SHA] ListAreaBlobsReq SHA
         */

        /**
         * Constructs a new ListAreaBlobsReq.
         * @memberof messages
         * @classdesc Represents a ListAreaBlobsReq.
         * @implements IListAreaBlobsReq
         * @constructor
         * @param {messages.IListAreaBlobsReq=} [properties] Properties to set
         */
        function ListAreaBlobsReq(properties) {
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * ListAreaBlobsReq SHA.
         * @member {string} SHA
         * @memberof messages.ListAreaBlobsReq
         * @instance
         */
        ListAreaBlobsReq.prototype.SHA = "";

        /**
         * Verifies a ListAreaBlobsReq message.
         * @function verify
         * @memberof messages.ListAreaBlobsReq
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        ListAreaBlobsReq.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                if (!$util.isString(message.SHA))
                    return "SHA: string expected";
            return null;
        };

        /**
         * Creates a ListAreaBlobsReq message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.ListAreaBlobsReq
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.ListAreaBlobsReq} ListAreaBlobsReq
         */
        ListAreaBlobsReq.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.ListAreaBlobsReq)
                return object;
            var message = new $root.messages.ListAreaBlobsReq();
            if (object.SHA != null)
                message.SHA = String(object.SHA);
            return message;
        };

        /**
         * Creates a plain object from a ListAreaBlobsReq message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.ListAreaBlobsReq
         * @static
         * @param {messages.ListAreaBlobsReq} message ListAreaBlobsReq
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        ListAreaBlobsReq.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.defaults)
                object.SHA = "";
            if (message.SHA != null && message.hasOwnProperty("SHA"))
                object.SHA = message.SHA;
            return object;
        };

        /**
         * Converts this ListAreaBlobsReq to JSON.
         * @function toJSON
         * @memberof messages.ListAreaBlobsReq
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        ListAreaBlobsReq.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return ListAreaBlobsReq;
    })();

    messages.ListAreaBlobsResp = (function() {

        /**
         * Properties of a ListAreaBlobsResp.
         * @memberof messages
         * @interface IListAreaBlobsResp
         * @property {Array.<messages.IBlobRecord>|null} [BlobRecords] ListAreaBlobsResp BlobRecords
         */

        /**
         * Constructs a new ListAreaBlobsResp.
         * @memberof messages
         * @classdesc Represents a ListAreaBlobsResp.
         * @implements IListAreaBlobsResp
         * @constructor
         * @param {messages.IListAreaBlobsResp=} [properties] Properties to set
         */
        function ListAreaBlobsResp(properties) {
            this.BlobRecords = [];
            if (properties)
                for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                    if (properties[keys[i]] != null)
                        this[keys[i]] = properties[keys[i]];
        }

        /**
         * ListAreaBlobsResp BlobRecords.
         * @member {Array.<messages.IBlobRecord>} BlobRecords
         * @memberof messages.ListAreaBlobsResp
         * @instance
         */
        ListAreaBlobsResp.prototype.BlobRecords = $util.emptyArray;

        /**
         * Verifies a ListAreaBlobsResp message.
         * @function verify
         * @memberof messages.ListAreaBlobsResp
         * @static
         * @param {Object.<string,*>} message Plain object to verify
         * @returns {string|null} `null` if valid, otherwise the reason why it is not
         */
        ListAreaBlobsResp.verify = function verify(message) {
            if (typeof message !== "object" || message === null)
                return "object expected";
            if (message.BlobRecords != null && message.hasOwnProperty("BlobRecords")) {
                if (!Array.isArray(message.BlobRecords))
                    return "BlobRecords: array expected";
                for (var i = 0; i < message.BlobRecords.length; ++i) {
                    var error = $root.messages.BlobRecord.verify(message.BlobRecords[i]);
                    if (error)
                        return "BlobRecords." + error;
                }
            }
            return null;
        };

        /**
         * Creates a ListAreaBlobsResp message from a plain object. Also converts values to their respective internal types.
         * @function fromObject
         * @memberof messages.ListAreaBlobsResp
         * @static
         * @param {Object.<string,*>} object Plain object
         * @returns {messages.ListAreaBlobsResp} ListAreaBlobsResp
         */
        ListAreaBlobsResp.fromObject = function fromObject(object) {
            if (object instanceof $root.messages.ListAreaBlobsResp)
                return object;
            var message = new $root.messages.ListAreaBlobsResp();
            if (object.BlobRecords) {
                if (!Array.isArray(object.BlobRecords))
                    throw TypeError(".messages.ListAreaBlobsResp.BlobRecords: array expected");
                message.BlobRecords = [];
                for (var i = 0; i < object.BlobRecords.length; ++i) {
                    if (typeof object.BlobRecords[i] !== "object")
                        throw TypeError(".messages.ListAreaBlobsResp.BlobRecords: object expected");
                    message.BlobRecords[i] = $root.messages.BlobRecord.fromObject(object.BlobRecords[i]);
                }
            }
            return message;
        };

        /**
         * Creates a plain object from a ListAreaBlobsResp message. Also converts values to other types if specified.
         * @function toObject
         * @memberof messages.ListAreaBlobsResp
         * @static
         * @param {messages.ListAreaBlobsResp} message ListAreaBlobsResp
         * @param {$protobuf.IConversionOptions} [options] Conversion options
         * @returns {Object.<string,*>} Plain object
         */
        ListAreaBlobsResp.toObject = function toObject(message, options) {
            if (!options)
                options = {};
            var object = {};
            if (options.arrays || options.defaults)
                object.BlobRecords = [];
            if (message.BlobRecords && message.BlobRecords.length) {
                object.BlobRecords = [];
                for (var j = 0; j < message.BlobRecords.length; ++j)
                    object.BlobRecords[j] = $root.messages.BlobRecord.toObject(message.BlobRecords[j], options);
            }
            return object;
        };

        /**
         * Converts this ListAreaBlobsResp to JSON.
         * @function toJSON
         * @memberof messages.ListAreaBlobsResp
         * @instance
         * @returns {Object.<string,*>} JSON object
         */
        ListAreaBlobsResp.prototype.toJSON = function toJSON() {
            return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
        };

        return ListAreaBlobsResp;
    })();

    return messages;
})();

module.exports = $root;
