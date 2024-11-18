import eslint from "@eslint/js";
import tseslint from "typescript-eslint";
import prettier from "eslint-config-prettier";

export default tseslint.config(
	eslint.configs.recommended,
	...tseslint.configs.recommended,
	prettier,
	{
		rules: {
			"eol-last": "error",
			"no-trailing-spaces": "error",
			"no-empty": "off",
			"@/explicit-function-return-type": "off",
			"@/no-unused-vars": "warn",
			"@/no-var-requires": "off",
			"@/no-explicit-any": "off",
			"@/ban-types": "off",
			"@/semi": ["error", "always"]
		},
		ignores: ["src/migration/*", "build/*"]
	}
);
